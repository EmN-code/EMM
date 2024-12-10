
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

void read_mma8x5x(const struct device *i2c_dev, int16_t *accel_data) {
    uint8_t data[6];
    int ret;

    ret = i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to read from MMA8x5x: %d", ret);
        return;
    }

    accel_data[0] = (int16_t)((data[0] << 8) | data[1]);
    accel_data[1] = (int16_t)((data[2] << 8) | data[3]);
    accel_data[2] = (int16_t)((data[4] << 8) | data[5]);
}

void store_accel_data(struct nvs_fs *fs, int16_t *accel_data) {
    int ret;

    ret = nvs_write(fs, NVS_KEY, accel_data, sizeof(accel_data));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS: %d", ret);
    } else {
        LOG_INF("Accel data stored in NVS");
    }
}

int main(void) {
    const struct device *i2c_dev;
    struct nvs_fs fs;
    int16_t accel_data[3];

    i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    fs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION);
    fs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION);
    fs.sector_size = 4096;
    fs.id_size = 2;

    int ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("NVS mount failed: %d", ret);
        return -1;
    }

    while (1) {
        read_mma8x5x(i2c_dev, accel_data);
        LOG_INF("Accel data: X=%d, Y=%d, Z=%d", accel_data[0], accel_data[1], accel_data[2]);

        store_accel_data(&fs, accel_data);

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
