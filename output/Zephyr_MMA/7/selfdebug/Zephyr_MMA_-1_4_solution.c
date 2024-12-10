
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_DEVICE_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION FIXED_PARTITION_ID(storage_partition)

LOG_MODULE_REGISTER(main);

void read_sensor_and_store(const struct device *dev, struct nvs_fs *fs)
{
    struct sensor_value accel[3];
    int ret;

    if (!device_is_ready(dev)) {
        LOG_ERR("MMA8x5x device not ready");
        return;
    }

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
    ret = nvs_write(fs, 0, &accel, sizeof(accel));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Sensor data stored in NVS");
    }
}

int main(void)
{
    const struct device *dev = device_get_binding(MMA8x5x_DEVICE_NAME);
    struct nvs_fs fs;
    int ret;

    if (!dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -ENODEV;
    }

    // Initialize NVS
    fs.flash_device = NVS_PARTITION;
    fs.offset = 0;
    fs.sector_size = 4096;
    fs.sector_count = 2;

    ret = nvs_init(&fs, fs.flash_device);
    if (ret) {
        LOG_ERR("Failed to initialize NVS (%d)", ret);
        return ret;
    }

    while (1) {
        read_sensor_and_store(dev, &fs);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
