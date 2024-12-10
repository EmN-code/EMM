
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION FLASH_AREA_ID(storage)
#define NVS_PARTITION_SIZE 0x6000
#define NVS_NAMESPACE "MMA8x5x"
#define NVS_KEY "accel_data"

static struct nvs_fs nvs = {
    .sector_size = NVS_PARTITION_SIZE,
    .sector_count = 1,
    .offset = 0,
};

static const struct device *i2c_dev;

static int read_mma8x5x(int16_t *accel_data)
{
    uint8_t data[2];
    int ret;

    ret = i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, &data[0]);
    if (ret < 0) {
        LOG_ERR("Failed to read MMA8x5x X MSB register");
        return ret;
    }

    ret = i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB + 1, &data[1]);
    if (ret < 0) {
        LOG_ERR("Failed to read MMA8x5x X LSB register");
        return ret;
    }

    *accel_data = (data[0] << 8) | data[1];
    return 0;
}

static int store_accel_data(int16_t accel_data)
{
    int ret;
    struct nvs_fs *fs = &nvs;

    ret = nvs_write(fs, NVS_KEY, &accel_data, sizeof(accel_data));
    if (ret < 0) {
        LOG_ERR("Failed to write accel data to NVS");
        return ret;
    }

    LOG_INF("Accel data stored: %d", accel_data);
    return 0;
}

int main(void)
{
    int16_t accel_data;
    int ret;

    i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    ret = nvs_mount(&nvs);
    if (ret) {
        LOG_ERR("NVS mount failed: %d", ret);
        return -1;
    }

    while (1) {
        ret = read_mma8x5x(&accel_data);
        if (ret < 0) {
            LOG_ERR("Failed to read MMA8x5x");
        } else {
            LOG_INF("Accel data read: %d", accel_data);
            store_accel_data(accel_data);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
