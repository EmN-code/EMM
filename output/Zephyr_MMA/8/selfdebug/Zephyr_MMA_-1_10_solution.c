
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_PARTITION FIXED_PARTITION_DEVICE(storage_partition)
#define NVS_SECTOR_SIZE 4096
#define NVS_NUM_SECTORS 4

LOG_MODULE_REGISTER(main);

struct nvs_fs fs = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = NVS_NUM_SECTORS,
    .offset = FIXED_PARTITION_OFFSET(storage_partition),
};

void read_mma8x5x(const struct device *i2c_dev, int16_t *x, int16_t *y, int16_t *z) {
    uint8_t buf[6];
    struct i2c_msg msgs[2] = {
        {
            .buf = &buf[0],
            .len = 1,
            .flags = I2C_MSG_WRITE | I2C_MSG_RESTART,
        },
        {
            .buf = &buf[1],
            .len = 5,
            .flags = I2C_MSG_READ | I2C_MSG_STOP,
        },
    };

    buf[0] = MMA8x5x_REG_OUT_X_MSB;

    if (i2c_transfer(i2c_dev, msgs, 2, MMA8x5x_I2C_ADDRESS) != 0) {
        LOG_ERR("Failed to read from MMA8x5x");
        return;
    }

    *x = (buf[1] << 8) | buf[2];
    *y = (buf[3] << 8) | buf[4];
    *z = (buf[5] << 8) | buf[6];
}

void store_value_in_nvs(struct nvs_fs *fs, uint16_t id, int16_t value) {
    int ret = nvs_write(fs, id, &value, sizeof(value));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS: %d", ret);
    } else {
        LOG_INF("Value stored in NVS: %d", value);
    }
}

int main(void) {
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    int ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("NVS mount failed: %d", ret);
        return -1;
    }

    while (1) {
        int16_t x, y, z;
        read_mma8x5x(i2c_dev, &x, &y, &z);
        LOG_INF("MMA8x5x: x=%d, y=%d, z=%d", x, y, z);

        store_value_in_nvs(&fs, 1, x);
        store_value_in_nvs(&fs, 2, y);
        store_value_in_nvs(&fs, 3, z);

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
