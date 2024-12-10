
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION FLASH_AREA_ID(storage)

#define SLEEP_TIME_MS 5000

const struct device *i2c_dev;
struct nvs_fs fs;

int read_mma8x5x(int16_t *x, int16_t *y, int16_t *z) {
    uint8_t data[6];
    int ret;

    ret = i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data, 6);
    if (ret < 0) {
        printk("I2C read failed: %d\n", ret);
        return ret;
    }

    *x = (data[0] << 8) | data[1];
    *y = (data[2] << 8) | data[3];
    *z = (data[4] << 8) | data[5];

    return 0;
}

int init_nvs(void) {
    int ret;

    fs.flash_device = NULL;
    fs.offset = FLASH_AREA_OFFSET(storage);
    fs.sector_size = 4096;
    fs.sector_count = 8;

    ret = nvs_mount(&fs);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
        return ret;
    }

    return 0;
}

int store_value_in_nvs(int16_t value) {
    int ret;
    ret = nvs_write(&fs, 1, &value, sizeof(value));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
        return ret;
    }
    return 0;
}

int main(void) {
    int16_t x, y, z;
    int ret;

    i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    ret = init_nvs();
    if (ret) {
        return -1;
    }

    while (1) {
        ret = read_mma8x5x(&x, &y, &z);
        if (ret == 0) {
            printk("MMA8x5x: X=%d, Y=%d, Z=%d\n", x, y, z);
            store_value_in_nvs(x); // Store the X value persistently
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
