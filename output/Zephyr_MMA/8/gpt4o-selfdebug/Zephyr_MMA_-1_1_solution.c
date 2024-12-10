#include <zephyr.h>
#include <sys/printk.h>
#include <storage/flash_map.h>
#include <drivers/i2c.h>
#include <device.h>
#include <nvs/nvs.h>

#define MMA8x5x_I2C_ADDR 0x1D  // Define MMA8x5x I2C address
#define SENSOR_REG_ADDR 0x00   // Define a register address to read from
#define NVS_ID_DATA 1         // Define an NVS ID to store sensor data

void read_sensor_and_store(const struct device *i2c_dev, struct nvs_fs *nvs)
{
    uint8_t sensor_data;
    int ret;
    ret = i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDR, SENSOR_REG_ADDR, &sensor_data);
    if (ret == 0) {
        printk("Sensor data: %d\n", sensor_data);
        ret = nvs_write(nvs, NVS_ID_DATA, &sensor_data, sizeof(sensor_data));
        if (ret < 0) {
            printk("Error writing to NVS: %d\n", ret);
        }
    } else {
        printk("Failed to read sensor data, Error: %d\n", ret);
    }
}

void main(void)
{
    const struct device *i2c_dev;
    struct nvs_fs nvs;
    struct flash_sector sector;
    int ret;
    
    // Initialize I2C
    i2c_dev = device_get_binding(DT_LABEL(DT_NODELABEL(i2c0)));
    if (!i2c_dev) {
        printk("I2C: Device not found.\n");
        return;
    }
    
    // Setting up NVS
    nvs.offset = FLASH_AREA_OFFSET(storage);
    ret = flash_area_get_sectors(FLASH_AREA_ID(storage), &sector, NULL);
    if (ret < 0) {
        printk("Unable to get flash sector, Error: %d\n", ret);
        return;
    }
    nvs.sector_count = 1;
    nvs.sectors = &sector;
    ret = nvs_init(&nvs, "nvs");
    if (ret) {
        printk("Failed to initialize NVS, Error: %d\n", ret);
        return;
    }

    while (1) {
        read_sensor_and_store(i2c_dev, &nvs);
        k_sleep(K_SECONDS(5));
    }
}
