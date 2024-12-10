
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID 1

LOG_MODULE_REGISTER(main);

const struct device *i2c_dev;
struct nvs_fs nvs;

int read_mma8x5x(int16_t *x, int16_t *y, int16_t *z) {
    uint8_t data[6];
    int ret;

    ret = i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data, 6);
    if (ret < 0) {
        LOG_ERR("Failed to read from MMA8x5x: %d", ret);
        return ret;
    }

    *x = (data[0] << 8) | data[1];
    *y = (data[2] << 8) | data[3];
    *z = (data[4] << 8) | data[5];

    return 0;
}

int init_nvs(void) {
    int ret;

    nvs.sector_size = 4096;
    nvs.sector_count = 2;
    nvs.offset = FLASH_AREA_OFFSET(storage);

    ret = nvs_init(&nvs, "storage");
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return ret;
    }

    return 0;
}

int store_data_nvs(int16_t x, int16_t y, int16_t z) {
    int ret;
    uint32_t data = (x << 16) | (y << 8) | z;

    ret = nvs_write(&nvs, NVS_PARTITION_ID, &data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("NVS write failed: %d", ret);
        return ret;
    }

    return 0;
}

int main(void) {
    int16_t x, y, z;
    int ret;

    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    ret = init_nvs();
    if (ret) {
        return -1;
    }

    while (1) {
        ret = read_mma8x5x(&x, &y, &z);
        if (ret == 0) {
            LOG_INF("MMA8x5x: X=%d, Y=%d, Z=%d", x, y, z);
            store_data_nvs(x, y, z);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
