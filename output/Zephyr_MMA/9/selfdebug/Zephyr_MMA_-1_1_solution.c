
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION FLASH_AREA_ID(storage)
#define NVS_NAMESPACE "mma8x5x"
#define NVS_KEY "accel_data"

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs = {
    .sector_size = DT_SIZE_K(4),
    .sector_count = 2,
    .offset = 0,
};

static int read_mma8x5x(const struct device *i2c_dev, uint8_t *data)
{
    return i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data, 6);
}

static int store_data_to_nvs(uint8_t *data, size_t len)
{
    int ret;
    struct nvs_fs *fs_ptr = &fs;

    ret = nvs_init(fs_ptr, NVS_PARTITION);
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return ret;
    }

    ret = nvs_write(fs_ptr, NVS_KEY, data, len);
    if (ret < 0) {
        LOG_ERR("NVS write failed: %d", ret);
        return ret;
    }

    return 0;
}

int main(void)
{
    const struct device *i2c_dev;
    uint8_t accel_data[6];
    int ret;

    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("Could not get I2C device");
        return -1;
    }

    while (1) {
        ret = read_mma8x5x(i2c_dev, accel_data);
        if (ret) {
            LOG_ERR("Failed to read MMA8x5x: %d", ret);
        } else {
            LOG_INF("Read MMA8x5x data: %02x %02x %02x %02x %02x %02x",
                    accel_data[0], accel_data[1], accel_data[2],
                    accel_data[3], accel_data[4], accel_data[5]);

            ret = store_data_to_nvs(accel_data, sizeof(accel_data));
            if (ret) {
                LOG_ERR("Failed to store data to NVS: %d", ret);
            } else {
                LOG_INF("Data stored to NVS successfully");
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}