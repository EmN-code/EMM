
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID 1
#define NVS_NAMESPACE "MMA8x5xData"
#define NVS_KEY "accel_data"

LOG_MODULE_REGISTER(main);

const struct device *i2c_dev;
struct nvs_fs nvs;

int read_mma8x5x(int16_t *accel_data) {
    uint8_t data[6];
    int ret;

    ret = i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data, 6);
    if (ret) {
        LOG_ERR("Failed to read from MMA8x5x: %d", ret);
        return ret;
    }

    accel_data[0] = (data[0] << 8) | data[1];
    accel_data[1] = (data[2] << 8) | data[3];
    accel_data[2] = (data[4] << 8) | data[5];

    return 0;
}

int store_to_nvs(int16_t *accel_data) {
    int ret;
    struct flash_sector sector;

    ret = flash_get_sector_info_by_id(NVS_PARTITION_ID, 0, &sector);
    if (ret) {
        LOG_ERR("Failed to get sector info: %d", ret);
        return ret;
    }

    nvs.offset = sector.fs_off;
    nvs.sector_size = sector.fs_size;
    nvs.sector_count = 1;

    ret = nvs_init(&nvs, "nvs");
    if (ret) {
        LOG_ERR("Failed to initialize NVS: %d", ret);
        return ret;
    }

    ret = nvs_write(&nvs, NVS_KEY, accel_data, sizeof(accel_data));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS: %d", ret);
        return ret;
    }

    return 0;
}

int main(void) {
    int16_t accel_data[3];
    int ret;

    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    while (1) {
        ret = read_mma8x5x(accel_data);
        if (ret) {
            LOG_ERR("Failed to read MMA8x5x: %d", ret);
            continue;
        }

        LOG_INF("Accelerometer data: X=%d, Y=%d, Z=%d", accel_data[0], accel_data[1], accel_data[2]);

        ret = store_to_nvs(accel_data);
        if (ret) {
            LOG_ERR("Failed to store data to NVS: %d", ret);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
