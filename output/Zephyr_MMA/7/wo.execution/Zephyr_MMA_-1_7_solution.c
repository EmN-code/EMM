
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs = {
    .sector_size = DT_FLASH_AREA_STORAGE_SIZE,
    .sector_count = DT_FLASH_AREA_STORAGE_NUM,
    .offset = DT_FLASH_AREA_STORAGE_OFFSET,
};

int read_mma8x5x(const struct device *i2c_dev, uint8_t *value)
{
    return i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, value);
}

int main(void)
{
    const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return -1;
    }

    int ret = nvs_init(&fs, DT_FLASH_AREA_STORAGE_DEV);
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return -1;
    }

    while (1) {
        uint8_t value;
        ret = read_mma8x5x(i2c_dev, &value);
        if (ret) {
            LOG_ERR("Failed to read MMA8x5x: %d", ret);
        } else {
            ret = nvs_write(&fs, 0, &value, sizeof(value));
            if (ret < 0) {
                LOG_ERR("NVS write failed: %d", ret);
            } else {
                LOG_INF("MMA8x5x value: %d", value);
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
