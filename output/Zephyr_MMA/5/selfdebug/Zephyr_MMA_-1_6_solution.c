
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <drivers/i2c.h>
#include <storage/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID 1

LOG_MODULE_REGISTER(main);

struct nvs_fs fs = {
    .sector_size = 4096,
    .sector_count = 2,
    .offset = 0,
};

void read_mma8x5x(const struct device *i2c_dev, int16_t *x, int16_t *y, int16_t *z) {
    uint8_t data[6];
    i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data, 6);

    *x = (data[0] << 8) | data[1];
    *y = (data[2] << 8) | data[3];
    *z = (data[4] << 8) | data[5];
}

void store_value_in_nvs(struct nvs_fs *fs, uint16_t id, int16_t value) {
    nvs_write(fs, id, &value, sizeof(value));
}

int main(void) {
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    int16_t x, y, z;

    if (nvs_init(&fs, "nvs") != 0) {
        LOG_ERR("NVS init failed");
        return -1;
    }

    while (1) {
        read_mma8x5x(i2c_dev, &x, &y, &z);
        LOG_INF("MMA8x5x: X=%d, Y=%d, Z=%d", x, y, z);

        store_value_in_nvs(&fs, NVS_PARTITION_ID, x);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
