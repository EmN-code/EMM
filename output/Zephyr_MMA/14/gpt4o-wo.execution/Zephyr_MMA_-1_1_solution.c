
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_I2C_ADDRESS 0x1D
#define NVS_PARTITION_ID 1
#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 2

static struct nvs_fs fs;

void read_mma8x5x(const struct device *dev, int16_t *x, int16_t *y, int16_t *z) {
    struct sensor_value accel[3];

    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Failed to fetch sample");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        LOG_ERR("Failed to get sensor data");
        return;
    }

    *x = accel[0].val1;
    *y = accel[1].val1;
    *z = accel[2].val1;
}

void store_reading(int16_t x, int16_t y, int16_t z) {
    struct nvs_fs *fs_ptr = &fs;
    int rc;

    rc = nvs_write(fs_ptr, 1, &x, sizeof(x));
    if (rc < 0) {
        LOG_ERR("Failed to write x value to NVS: %d", rc);
    }

    rc = nvs_write(fs_ptr, 2, &y, sizeof(y));
    if (rc < 0) {
        LOG_ERR("Failed to write y value to NVS: %d", rc);
    }

    rc = nvs_write(fs_ptr, 3, &z, sizeof(z));
    if (rc < 0) {
        LOG_ERR("Failed to write z value to NVS: %d", rc);
    }
}

int main(void) {
    const struct device *mma8x5x_dev;
    int16_t x, y, z;
    int rc;

    mma8x5x_dev = device_get_binding("MMA8x5x");
    if (!mma8x5x_dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -1;
    }

    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;
    fs.offset = FLASH_AREA_OFFSET(storage);

    rc = nvs_mount(&fs);
    if (rc) {
        LOG_ERR("NVS mount failed: %d", rc);
        return -1;
    }

    while (1) {
        read_mma8x5x(mma8x5x_dev, &x, &y, &z);
        LOG_INF("MMA8x5x: x=%d, y=%d, z=%d", x, y, z);
        store_reading(x, y, z);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
