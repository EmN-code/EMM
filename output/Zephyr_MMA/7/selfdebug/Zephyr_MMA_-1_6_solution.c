
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define MMA8x5x_REG_OUT_Y_MSB 0x03
#define MMA8x5x_REG_OUT_Z_MSB 0x05

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_SECTOR_SIZE 4096

LOG_MODULE_REGISTER(main);

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
    const struct device *i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("Could not get I2C device");
        return -1;
    }

    struct nvs_fs fs;
    fs.offset = FIXED_PARTITION_ID(storage_partition);
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = 1;

    int ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("NVS mount failed: %d", ret);
        return -1;
    }

    while (1) {
        int16_t x, y, z;
        read_mma8x5x(i2c_dev, &x, &y, &z);

        LOG_INF("MMA8x5x: X=%d, Y=%d, Z=%d", x, y, z);

        store_value_in_nvs(&fs, 1, x);
        store_value_in_nvs(&fs, 2, y);
        store_value_in_nvs(&fs, 3, z);

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
