
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

LOG_MODULE_REGISTER(main);

void read_mma8x5x(const struct device *i2c_dev, int16_t *x_axis) {
    uint8_t data[2];
    i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, &data[0]);
    i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB + 1, &data[1]);
    *x_axis = (data[0] << 8) | data[1];
}

int main(void) {
    const struct device *i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("Could not get I2C device");
        return -1;
    }

    struct nvs_fs fs;
    fs.flash_device = FIXED_PARTITION_DEVICE(storage);
    fs.offset = FIXED_PARTITION_OFFSET(storage);
    fs.sector_size = 4096;
    fs.sector_count = 2;

    int ret = nvs_init(&fs, "storage");
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return -1;
    }

    int16_t x_axis;
    while (1) {
        read_mma8x5x(i2c_dev, &x_axis);
        LOG_INF("X-axis: %d", x_axis);

        ret = nvs_write(&fs, 1, &x_axis, sizeof(x_axis));
        if (ret < 0) {
            LOG_ERR("NVS write failed: %d", ret);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
