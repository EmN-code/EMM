
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define STORAGE_NODE DT_NODELABEL(storage_partition)
#define STORAGE_SIZE DT_REG_SIZE(STORAGE_NODE)
#define STORAGE_OFFSET DT_REG_ADDR(STORAGE_NODE)

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs = {
    .sector_size = STORAGE_SIZE,
    .sector_count = 1,
    .offset = STORAGE_OFFSET,
};

int read_mma8x5x(const struct device *i2c_dev, uint8_t *value)
{
    return i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, value);
}

int main(void)
{
    const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c_0));
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return -1;
    }

    int ret = nvs_init(&fs, DT_LABEL(STORAGE_NODE));
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return -1;
    }

    uint8_t value;
    while (1) {
        ret = read_mma8x5x(i2c_dev, &value);
        if (ret) {
            LOG_ERR("Failed to read MMA8x5x: %d", ret);
        } else {
            LOG_INF("Read value: %d", value);
            ret = nvs_write(&fs, 0, &value, sizeof(value));
            if (ret < 0) {
                LOG_ERR("NVS write failed: %d", ret);
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
