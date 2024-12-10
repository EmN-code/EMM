
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/fs.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_I2C_ADDRESS 0x1D
#define NVS_PARTITION_ID 1
#define NVS_STORAGE_NAME "storage"

static struct nvs_fs nvs = {
    .sector_size = DT_FLASH_AREA_STORAGE_SIZE,
    .sector_count = 4,
    .offset = DT_FLASH_AREA_STORAGE_OFFSET,
};

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

    // Store the reading value persistently using NVS
    struct fs_file_t file;
    fs_file_t_init(&file);

    ret = fs_open(&file, NVS_STORAGE_NAME, FS_O_CREATE | FS_O_WRITE);
    if (ret) {
        LOG_ERR("Failed to open NVS file (%d)", ret);
        return;
    }

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "X: %d.%06d, Y: %d.%06d, Z: %d.%06d",
             accel[0].val1, accel[0].val2,
             accel[1].val1, accel[1].val2,
             accel[2].val1, accel[2].val2);

    ret = fs_write(&file, buffer, strlen(buffer));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS file (%d)", ret);
    }

    fs_close(&file);
}

int main(void) {
    const struct device *mma8x5x_dev = device_get_binding("MMA8x5x");
    if (!mma8x5x_dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -1;
    }

    int ret = nvs_init(&nvs, NVS_PARTITION_ID);
    if (ret) {
        LOG_ERR("Failed to initialize NVS (%d)", ret);
        return -1;
    }

    while (1) {
        read_mma8x5x(mma8x5x_dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
