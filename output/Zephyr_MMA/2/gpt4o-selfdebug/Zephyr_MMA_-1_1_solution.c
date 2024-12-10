#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <storage/flash_area.h>
#include <storage/nvs.h>

#define MMA8x5x_I2C_ADDR 0x1C
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define STORAGE_ID CONFIG_NVS_STORAGE_ID
#define NVS_KEY_SENSOR_DATA 1

void main(void) {
    const struct device *i2c_dev;
    struct nvs_fs nvs;
    struct flash_area *flash_area;
    uint8_t sensor_data[6];
    int rc;

    i2c_dev = device_get_binding(DT_LABEL(DT_NODELABEL(i2c0)));
    if (!i2c_dev) {
        printk("I2C: Device not found.\n");
        return;
    }

    rc = flash_area_open(STORAGE_ID, &flash_area);
    if (rc < 0) {
        printk("NVS: Flash area open failed.\n");
        return;
    }

    nvs.flash_area = flash_area;
    nvs.sector_size = FLASH_AREA_SIZE;
    nvs.sector_count = FLASH_AREA_COUNT;

    rc = nvs_init(&nvs, STORAGE_ID);
    if (rc < 0) {
        printk("NVS: Initialization failed.\n");
        return;
    }

    while (1) {
        rc = i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDR, MMA8x5x_REG_OUT_X_MSB, sensor_data, sizeof(sensor_data));
        if (rc < 0) {
            printk("I2C: Data read failed.\n");
        } else {
            printk("Sensor data: %d %d %d %d %d %d\n", sensor_data[0], sensor_data[1], sensor_data[2], sensor_data[3], sensor_data[4], sensor_data[5]);
            rc = nvs_write(&nvs, NVS_KEY_SENSOR_DATA, sensor_data, sizeof(sensor_data));
            if (rc < 0) {
                printk("NVS: Data write failed.\n");
            }
        }

        k_sleep(K_SECONDS(5));
    }
}