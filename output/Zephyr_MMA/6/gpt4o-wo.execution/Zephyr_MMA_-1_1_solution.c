
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

void read_mma8x5x(const struct device *dev, struct sensor_value *accel) {
    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Failed to fetch sample");
        return;
    }

    sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
}

void store_reading(int16_t reading) {
    struct nvs_fs *fs_ptr = &fs;
    int rc;

    rc = nvs_write(fs_ptr, 0, &reading, sizeof(reading));
    if (rc < 0) {
        LOG_ERR("Failed to write to NVS: %d", rc);
    } else {
        LOG_INF("Stored reading: %d", reading);
    }
}

int main(void) {
    const struct device *i2c_dev = device_get_binding(MMA8x5x_I2C_BUS);
    if (!i2c_dev) {
        LOG_ERR("Could not get I2C device");
        return -1;
    }

    const struct device *mma8x5x_dev = device_get_binding("MMA8x5x");
    if (!mma8x5x_dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -1;
    }

    fs.flash_device = NVS_PARTITION;
    fs.offset = 0;
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;

    int rc = nvs_mount(&fs);
    if (rc) {
        LOG_ERR("Failed to mount NVS: %d", rc);
        return -1;
    }

    struct sensor_value accel[3];

    while (1) {
        read_mma8x5x(mma8x5x_dev, accel);
        int16_t reading = (int16_t)(accel[0].val1); // Simplified for example
        store_reading(reading);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
