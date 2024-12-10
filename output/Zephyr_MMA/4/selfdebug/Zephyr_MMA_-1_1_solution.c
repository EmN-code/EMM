
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID 1
#define NVS_STORAGE_ID 1

const struct device *i2c_dev;
struct nvs_fs fs;

int read_mma8x5x(int16_t *x_value) {
    uint8_t data[2];
    int ret;

    ret = i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, &data[0]);
    if (ret < 0) {
        printk("Failed to read from MMA8x5x\n");
        return ret;
    }

    ret = i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB + 1, &data[1]);
    if (ret < 0) {
        printk("Failed to read from MMA8x5x\n");
        return ret;
    }

    *x_value = (data[0] << 8) | data[1];
    return 0;
}

int init_nvs(void) {
    int ret;

    fs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION_ID);
    if (!device_is_ready(fs.flash_device)) {
        printk("Flash device not ready\n");
        return -ENODEV;
    }

    fs.offset = FLASH_AREA_OFFSET(NVS_PARTITION_ID);
    fs.sector_size = 4096;
    fs.sector_count = 2;

    ret = nvs_init(&fs, "storage");
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return ret;
    }

    return 0;
}

int store_value_in_nvs(int16_t value) {
    int ret;

    ret = nvs_write(&fs, NVS_STORAGE_ID, &value, sizeof(value));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
        return ret;
    }

    return 0;
}

int main(void) {
    int16_t x_value;
    int ret;

    i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -ENODEV;
    }

    ret = init_nvs();
    if (ret < 0) {
        return ret;
    }

    while (1) {
        ret = read_mma8x5x(&x_value);
        if (ret < 0) {
            printk("Failed to read MMA8x5x\n");
        } else {
            printk("MMA8x5x X value: %d\n", x_value);
            store_value_in_nvs(x_value);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
