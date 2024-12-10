
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_DEVICE_NAME "MMA8x5x"
#define NVS_PARTITION_ID 0
#define NVS_PARTITION_LABEL "storage"

LOG_MODULE_REGISTER(main);

void read_sensor_and_store(const struct device *dev, struct nvs_fs *fs) {
    struct sensor_value accel[3];
    int ret;

    // Read sensor data
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

    // Store the sensor data in NVS
    ret = nvs_write(fs, NVS_PARTITION_ID, &accel, sizeof(accel));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Stored sensor data in NVS");
    }
}

int main(void) {
    const struct device *dev;
    struct nvs_fs fs;
    int ret;

    // Initialize NVS
    fs.flash_device = FIXED_PARTITION_DEVICE(NVS_PARTITION_LABEL);
    fs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_LABEL);
    fs.sector_size = 4096; // Example sector size, adjust as needed
    fs.sector_count = 2;   // Example sector count, adjust as needed

    ret = nvs_init(&fs, NVS_PARTITION_LABEL);
    if (ret) {
        LOG_ERR("Failed to initialize NVS (%d)", ret);
        return ret;
    }

    // Get the MMA8x5x device
    dev = device_get_binding(MMA8x5x_DEVICE_NAME);
    if (!dev) {
        LOG_ERR("Failed to get device binding for %s", MMA8x5x_DEVICE_NAME);
        return -ENODEV;
    }

    // Read sensor and store in NVS every 5 seconds
    while (1) {
        read_sensor_and_store(dev, &fs);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
