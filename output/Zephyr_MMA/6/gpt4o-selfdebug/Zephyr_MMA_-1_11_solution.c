#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>

#define I2C_DEV DT_LABEL(DT_NODELABEL(i2c0))
#define MMA85XX_ADDR 0x1D
#define MMA85XX_REG_OUT_X 0x01
#define STORAGE_ID 1
#define READ_INTERVAL_MS 5000

static struct nvs_fs fs;

void read_mma85xx(const struct device *i2c_dev) {
    uint8_t reg_addr = MMA85XX_REG_OUT_X;
    uint8_t sensor_data[6]; // Assume 3-axis, 2 bytes each
    struct nvs_fs fs;
    int ret;

    ret = i2c_write_read(i2c_dev, MMA85XX_ADDR,
                         &reg_addr, sizeof(reg_addr),
                         sensor_data, sizeof(sensor_data));
    if (ret < 0) {
        printk("Failed to read sensor: %d\n", ret);
        return;
    }

    // Store the sensor data in NVS
    ret = nvs_write(&fs, STORAGE_ID, &sensor_data, sizeof(sensor_data));
    if (ret < 0) {
        printk("Failed to write sensor data to NVS: %d\n", ret);
    }
}

void main(void) {
    const struct device *i2c_dev = device_get_binding(I2C_DEV);
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return;
    }

    int ret;
    struct flash_area *fa;

    ret = flash_area_open(FLASH_AREA_ID(storage), &fa);
    if (ret) {
        printk("Flash area open failed\n");
        return;
    }

    fs.offset = fa->fa_off;
    fs.sector_size = 4096;
    fs.sector_count = 3;

    ret = nvs_init(&fs, DT_LABEL(DT_NODELABEL(flash_contoller)));
    if (ret) {
        printk("NVS Initialization failed\n");
        return;
    }

    while (1) {
        read_mma85xx(i2c_dev);
        k_msleep(READ_INTERVAL_MS);
    }
}