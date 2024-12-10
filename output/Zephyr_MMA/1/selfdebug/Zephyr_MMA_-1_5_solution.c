
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_DEVICE_NAME "MMA8x5x"
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_SECTOR_SIZE 4096

LOG_MODULE_REGISTER(main);

void read_sensor_and_store(const struct device *dev, struct nvs_fs *fs)
{
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

    // Store the sensor data in NVS
    ret = nvs_write(fs, 1, &accel, sizeof(accel));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Sensor data stored in NVS");
    }
}

int main(void)
{
    const struct device *dev;
    struct nvs_fs fs;
    int ret;

    // Initialize NVS
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = 1;
    fs.offset = FIXED_PARTITION_OFFSET(storage_partition);

    ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("Failed to mount NVS (%d)", ret);
        return -1;
    }

    // Get the MMA8x5x device
    dev = device_get_binding(MMA8x5x_DEVICE_NAME);
    if (!dev) {
        LOG_ERR("Failed to get device binding");
        return -1;
    }

    while (1) {
        read_sensor_and_store(dev, &fs);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
