
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID 1

static struct nvs_fs nvs = {
    .sector_size = 4096,
    .sector_count = 2,
    .offset = 0,
};

static const struct device *i2c_dev;

static int read_mma8x5x(int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t data[6];
    int ret;

    ret = i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data, 6);
    if (ret < 0) {
        LOG_ERR("Failed to read from MMA8x5x sensor");
        return ret;
    }

    *x = (data[0] << 8) | data[1];
    *y = (data[2] << 8) | data[3];
    *z = (data[4] << 8) | data[5];

    return 0;
}

static int store_value_in_nvs(int16_t value)
{
    int ret;
    ret = nvs_write(&nvs, 0, &value, sizeof(value));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS");
        return ret;
    }
    return 0;
}

int main(void)
{
    int16_t x, y, z;
    int ret;

    i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    ret = nvs_init(&nvs, "storage");
    if (ret) {
        LOG_ERR("NVS init failed");
        return -1;
    }

    while (1) {
        ret = read_mma8x5x(&x, &y, &z);
        if (ret == 0) {
            LOG_INF("MMA8x5x: X=%d, Y=%d, Z=%d", x, y, z);
            store_value_in_nvs(x); // Store only X value for simplicity
        }
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
