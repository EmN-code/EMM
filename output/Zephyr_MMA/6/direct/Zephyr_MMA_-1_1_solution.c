
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8X5X_NODE DT_ALIAS(mma8x5x)
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

static struct nvs_fs fs = {
    .sector_size = DT_SIZE_K(4),
    .sector_count = 2,
    .offset = FIXED_PARTITION_OFFSET(storage_partition),
};

void store_reading(int16_t reading) {
    int rc;
    struct nvs_fs *fs_ptr = &fs;

    rc = nvs_init(fs_ptr, NVS_PARTITION_ID);
    if (rc) {
        LOG_ERR("NVS init failed: %d", rc);
        return;
    }

    rc = nvs_write(fs_ptr, 0, &reading, sizeof(reading));
    if (rc < 0) {
        LOG_ERR("NVS write failed: %d", rc);
    } else {
        LOG_INF("Stored reading: %d", reading);
    }
}

int main(void) {
    const struct device *dev;
    struct sensor_value accel[3];
    int16_t reading;

    dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s is not ready", dev->name);
        return -ENODEV;
    }

    while (1) {
        if (sensor_sample_fetch(dev) < 0) {
            LOG_ERR("Failed to fetch sample");
            continue;
        }

        if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
            LOG_ERR("Failed to get sensor data");
            continue;
        }

        reading = (int16_t)(accel[0].val1); // Assuming we are interested in the X-axis reading
        store_reading(reading);

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
