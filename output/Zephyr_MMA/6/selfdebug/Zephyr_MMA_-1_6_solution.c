
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION FLASH_AREA_ID(storage)

LOG_MODULE_REGISTER(main);

void read_mma8x5x(const struct device *i2c_dev, int16_t *x, int16_t *y, int16_t *z) {
    uint8_t data[6];
    struct i2c_msg msgs[2];

    msgs[0].buf = &data[0];
    msgs[0].len = 1;
    msgs[0].flags = I2C_MSG_WRITE;

    msgs[1].buf = &data[1];
    msgs[1].len = 5;
    msgs[1].flags = I2C_MSG_READ | I2C_MSG_RESTART;

    data[0] = MMA8x5x_REG_OUT_X_MSB;

    if (i2c_transfer(i2c_dev, msgs, 2, MMA8x5x_I2C_ADDRESS) != 0) {
        LOG_ERR("Failed to read from MMA8x5x");
        return;
    }

    *x = (data[0] << 8) | data[1];
    *y = (data[2] << 8) | data[3];
    *z = (data[4] << 8) | data[5];
}

void store_value_nvs(struct nvs_fs *fs, uint16_t id, int16_t value) {
    int ret;
    ret = nvs_write(fs, id, &value, sizeof(value));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (err %d)", ret);
    } else {
        LOG_INF("Stored value %d in NVS with ID %d", value, id);
    }
}

int main(void) {
    const struct device *i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    struct nvs_fs fs;
    fs.sector_size = 4096;
    fs.sector_count = 2;
    fs.offset = 0;
    fs.flash_device = FLASH_AREA_ID(storage);

    int ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("Failed to mount NVS (err %d)", ret);
        return -1;
    }

    int16_t x, y, z;
    while (1) {
        read_mma8x5x(i2c_dev, &x, &y, &z);
        LOG_INF("MMA8x5x: X=%d, Y=%d, Z=%d", x, y, z);

        store_value_nvs(&fs, 1, x);
        store_value_nvs(&fs, 2, y);
        store_value_nvs(&fs, 3, z);

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
