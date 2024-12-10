
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(storage_partition)

LOG_MODULE_REGISTER(main);

void read_sensor_and_store(const struct device *dev, struct nvs_fs *fs)
{
    struct sensor_value accel[3];
    int rc;

    if (!device_is_ready(dev)) {
        LOG_ERR("MMA8x5x device not ready");
        return;
    }

    rc = sensor_sample_fetch(dev);
    if (rc) {
        LOG_ERR("Failed to fetch sample (%d)", rc);
        return;
    }

    rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (rc) {
        LOG_ERR("Failed to get sensor data (%d)", rc);
        return;
    }

    // Store the read values in NVS
    rc = nvs_write(fs, 0, &accel, sizeof(accel));
    if (rc < 0) {
        LOG_ERR("Failed to write to NVS (%d)", rc);
    } else {
        LOG_INF("Stored sensor data in NVS");
    }
}

int main(void)
{
    const struct device *mma8x5x_dev = device_get_binding(MMA8x5x_DEV_NAME);
    struct nvs_fs fs;
    int rc;

    if (!mma8x5x_dev) {
        LOG_ERR("Failed to get MMA8x5x device");
        return -ENODEV;
    }

    // Initialize NVS
    fs.flash_device = NVS_PARTITION_DEVICE;
    fs.offset = FIXED_PARTITION_OFFSET(storage_partition);
    fs.sector_size = 4096; // Example sector size, adjust as needed
    fs.sector_count = 2;   // Example sector count, adjust as needed

    rc = nvs_init(&fs, NVS_PARTITION_ID);
    if (rc) {
        LOG_ERR("Failed to initialize NVS (%d)", rc);
        return rc;
    }

    while (1) {
        read_sensor_and_store(mma8x5x_dev, &fs);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
