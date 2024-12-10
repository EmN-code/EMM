
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_I2C_BUS DT_LABEL(DT_NODELABEL(i2c0))
#define NVS_PARTITION DT_LABEL(DT_ALIAS(storage_partition))
#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 2

static struct nvs_fs fs;

void read_mma8x5x(const struct device *dev) {
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret) {
        LOG_ERR("Failed to fetch sample (%d)", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Failed to get sensor data (%d)", ret);
        return;
    }

    LOG_INF("X: %d.%06d m/s^2, Y: %d.%06d m/s^2, Z: %d.%06d m/s^2",
            accel[0].val1, accel[0].val2,
            accel[1].val1, accel[1].val2,
            accel[2].val1, accel[2].val2);

    // Store the reading value persistently in NVS
    uint8_t data[12];
    memcpy(data, accel, sizeof(accel));
    ret = nvs_write(&fs, 1, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Data written to NVS");
    }
}

int main(void) {
    const struct device *mma8x5x_dev;
    int ret;

    // Initialize NVS
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;
    fs.offset = FLASH_AREA_OFFSET(storage);
    ret = nvs_init(&fs, NVS_PARTITION);
    if (ret) {
        LOG_ERR("Failed to initialize NVS (%d)", ret);
        return -1;
    }

    // Initialize MMA8x5x sensor
    mma8x5x_dev = device_get_binding(MMA8x5x_I2C_BUS);
    if (!mma8x5x_dev) {
        LOG_ERR("Failed to get MMA8x5x device");
        return -1;
    }

    while (1) {
        read_mma8x5x(mma8x5x_dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
