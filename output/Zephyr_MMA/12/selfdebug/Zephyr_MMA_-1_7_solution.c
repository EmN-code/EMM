
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

LOG_MODULE_REGISTER(main);

void read_mma8x5x(const struct device *i2c_dev, int16_t *x, int16_t *y, int16_t *z) {
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
        LOG_ERR("Failed to read from MMA8x5x");
        return;
    }

    *x = (data[0] << 8) | data[1];
    *y = (data[2] << 8) | data[3];
    *z = (data[4] << 8) | data[5];
}

void store_value_in_nvs(int16_t value) {
    struct nvs_fs fs;
    fs.flash_device = DEVICE_DT_GET(NVS_PARTITION_ID);
    fs.offset = 0;
    fs.sector_size = 4096;
    fs.sector_count = 1;

    if (nvs_mount(&fs) != 0) {
        LOG_ERR("Failed to mount NVS");
        return;
    }

    if (nvs_write(&fs, 1, &value, sizeof(value)) != sizeof(value)) {
        LOG_ERR("Failed to write to NVS");
    }

    nvs_unmount(&fs);
}

int main(void) {
    const struct device *i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    int16_t x, y, z;

    while (1) {
        read_mma8x5x(i2c_dev, &x, &y, &z);
        LOG_INF("MMA8x5x: X=%d, Y=%d, Z=%d", x, y, z);

        store_value_in_nvs(x);

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
