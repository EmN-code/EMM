
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_SECTOR_SIZE 4096

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = 1,
    .offset = FIXED_PARTITION_OFFSET(storage_partition),
};

void read_mma8x5x(const struct device *i2c_dev, int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t data[6];
    struct i2c_msg msgs[2];

    msgs[0].buf = &data[0];
    msgs[0].len = 1;
    msgs[0].flags = I2C_MSG_WRITE | I2C_MSG_RESTART;

    msgs[1].buf = &data[1];
    msgs[1].len = 5;
    msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;

    data[0] = MMA8x5x_REG_OUT_X_MSB;

    if (i2c_transfer(i2c_dev, msgs, 2, MMA8x5x_I2C_ADDRESS) != 0) {
        LOG_ERR("Failed to read from MMA8x5x sensor");
        return;
    }

    *x = (data[0] << 8) | data[1];
    *y = (data[2] << 8) | data[3];
    *z = (data[4] << 8) | data[5];
}

void store_in_nvs(int16_t x, int16_t y, int16_t z)
{
    int ret;
    uint8_t data[6];

    data[0] = (x >> 8) & 0xFF;
    data[1] = x & 0xFF;
    data[2] = (y >> 8) & 0xFF;
    data[3] = y & 0xFF;
    data[4] = (z >> 8) & 0xFF;
    data[5] = z & 0xFF;

    ret = nvs_write(&fs, 0, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS: %d", ret);
    } else {
        LOG_INF("Data written to NVS");
    }
}

void main(void)
{
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return;
    }

    int ret = nvs_init(&fs, NVS_PARTITION_ID);
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return;
    }

    while (1) {
        int16_t x, y, z;

        read_mma8x5x(i2c_dev, &x, &y, &z);
        LOG_INF("MMA8x5x: x=%d, y=%d, z=%d", x, y, z);

        store_in_nvs(x, y, z);

        k_sleep(K_SECONDS(5));
    }
}
