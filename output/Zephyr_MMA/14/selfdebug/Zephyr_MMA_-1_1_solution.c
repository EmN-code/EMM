
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
#define NVS_STORAGE_NAME "mma8x5x_data"

static struct nvs_fs nvs_config = {
    .sector_size = 4096,
    .sector_count = 2,
    .flash_device = NVS_PARTITION_ID,
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

static int store_data_to_nvs(int16_t x, int16_t y, int16_t z)
{
    struct nvs_fs *fs = &nvs_config;
    int ret;

    ret = nvs_mount(fs);
    if (ret) {
        LOG_ERR("Failed to mount NVS");
        return ret;
    }

    ret = nvs_write(fs, 1, &x, sizeof(x));
    if (ret < 0) {
        LOG_ERR("Failed to write X value to NVS");
        return ret;
    }

    ret = nvs_write(fs, 2, &y, sizeof(y));
    if (ret < 0) {
        LOG_ERR("Failed to write Y value to NVS");
        return ret;
    }

    ret = nvs_write(fs, 3, &z, sizeof(z));
    if (ret < 0) {
        LOG_ERR("Failed to write Z value to NVS");
        return ret;
    }

    return 0;
}

int main(void)
{
    int16_t x, y, z;
    int ret;

    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    ret = nvs_init(&nvs_config, NVS_STORAGE_NAME);
    if (ret) {
        LOG_ERR("Failed to initialize NVS");
        return ret;
    }

    while (1) {
        ret = read_mma8x5x(&x, &y, &z);
        if (ret == 0) {
            LOG_INF("MMA8x5x: X=%d, Y=%d, Z=%d", x, y, z);
            store_data_to_nvs(x, y, z);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
