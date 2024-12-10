#include <zephyr/sys/mutex.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/linker/sections.h>
#include <zephyr/kernel.h>
#include <zephyr/i2c.h>  // Ensure this path is correct and the file exists

#define I2C_DEV DT_LABEL(DT_NODELABEL(i2c0))
#define MMA8X5X_I2C_ADDR 0x1C  // Ensure your sensor's I2C address is correct
#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 3

static struct nvs_fs fs;
static const struct device *i2c_dev;

void read_mma8x5x_and_store_to_nvs(void)
{
    // Data read buffer
    uint8_t data[6];
    // Assuming MMA8x5x registers
    const uint8_t start_reg = 0x00;
    
    // Read 6 bytes from MMA8x5x
    int ret = i2c_burst_read(i2c_dev, MMA8X5X_I2C_ADDR, start_reg, data, sizeof(data));
    if (ret == 0) {
        // Data read successfully from the sensor
        // Store data persistently using NVS
        struct nvs_entry ent;
        ent.data = data;

        ret = nvs_write(&fs, 1, &ent, sizeof(ent));
        if (ret <= 0) {
            printk("Failed to write to NVS\n");
        }
    } else {
        printk("Failed to read from MMA8x5x\n");
    }
}

void main(void)
{
    int ret;
    
    // Initialize I2C device
    i2c_dev = device_get_binding(I2C_DEV);
    if (!i2c_dev) {
        printk("I2C: Device not found\n");
        return;
    }

    // NVS configuration
    fs.offset = FLASH_AREA_OFFSET(storage);
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;

    ret = nvs_init(&fs, storage);
    if (ret) {
        printk("Failed to initialize NVS\n");
        return;
    }

    while (1) {
        read_mma8x5x_and_store_to_nvs();
        k_sleep(K_SECONDS(5));
    }
}