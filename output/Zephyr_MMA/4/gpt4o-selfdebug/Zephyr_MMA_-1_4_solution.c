#include <zephyr/kernel.h>
#include <zephyr/i2c.h>
#include <zephyr/device.h>
#include <zephyr/nvs/nvs.h>

#define I2C_DEV_LABEL "I2C_0"
#define MMA8X5X_ADDR 0x1D // Typical I2C address for MMA8x5x
#define MMA8X5X_REG 0x00  // Change as per specific register of interest
#define STORAGE_FLASH_OFFSET 0x0000 // Adjust if needed

static struct nvs_fs nvs;
static const struct device *i2c_dev;

void mma8x5x_read_and_store(void) {
    uint8_t sensor_data;
    int rc;
    struct nvs_entry entry;

    // Read from sensor
    rc = i2c_reg_read_byte(i2c_dev, MMA8X5X_ADDR, MMA8X5X_REG, &sensor_data);
    if (rc != 0) {
        printk("I2C read failed
");
        return;
    }

    // Store to NVS
    entry.id = 1;
    rc = nvs_write(&nvs, entry.id, &sensor_data, sizeof(sensor_data));
    if (rc < 0) {
        printk("Failed to write sensor data to NVS
");
    }
}

void main(void) {
    struct flash_pages_info info;

    printk("Starting MMA8x5x Sensor Application\n");

    i2c_dev = device_get_binding(I2C_DEV_LABEL);
    if (!i2c_dev) {
        printk("I2C: Device not found.
");
        return;
    }

    nvs.storage_flash = device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
    if (!nvs.storage_flash) {
        printk("Flash device not found
");
        return;
    }

    if (flash_get_page_info_by_offs(nvs.storage_flash, STORAGE_FLASH_OFFSET, &info)) {
        printk("Storage Flash memory not supported on this platform
");
        return;
    }

    nvs.sector_size = info.size;
    nvs.sector_count = 3U; // Adjust the sector count if necessary
    nvs.offset = STORAGE_FLASH_OFFSET;

    int rc = nvs_init(&nvs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
    if (rc) {
        printk("Failed to initialize NVS
");
        return;
    }

    while (1) {
        mma8x5x_read_and_store();
        k_sleep(K_SECONDS(5));
    }
}