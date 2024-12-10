
#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

LOG_MODULE_REGISTER(main);

const struct device *i2c_dev;
struct nvs_fs fs;

int read_mma8x5x(uint8_t *value) {
    return i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, value);
}

int init_nvs(void) {
    fs.flash_device = FIXED_PARTITION_DEVICE(storage);
    fs.offset = FIXED_PARTITION_OFFSET(storage);
    fs.sector_size = 4096;
    fs.sector_count = 3;

    return nvs_init(&fs, "storage");
}

int store_value(uint8_t value) {
    return nvs_write(&fs, 0, &value, sizeof(value));
}

int main(void) {
    uint8_t mma8x5x_value;

    i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("Could not get I2C device");
        return -1;
    }

    if (init_nvs() != 0) {
        LOG_ERR("NVS init failed");
        return -1;
    }

    while (1) {
        if (read_mma8x5x(&mma8x5x_value) == 0) {
            LOG_INF("MMA8x5x value: %d", mma8x5x_value);
            if (store_value(mma8x5x_value) != 0) {
                LOG_ERR("Failed to store value in NVS");
            }
        } else {
            LOG_ERR("Failed to read MMA8x5x");
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
