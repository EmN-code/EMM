
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8X5X_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(NVS_PARTITION_ID)

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

    ret = nvs_write(fs, 0, &accel, sizeof(accel));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Successfully wrote sensor data to NVS");
    }
}

int main(void)
{
    const struct device *dev;
    struct nvs_fs fs;
    int ret;

    dev = device_get_binding(MMA8X5X_DEV_NAME);
    if (!dev) {
        LOG_ERR("Could not get MMA8X5X device");
        return -ENODEV;
    }

    fs.flash_device = NVS_PARTITION_DEVICE;
    fs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_ID);
    fs.sector_size = 4096;
    fs.sector_count = 2;

    ret = nvs_init(&fs, NVS_PARTITION_DEVICE);
    if (ret) {
        LOG_ERR("Failed to initialize NVS (%d)", ret);
        return -EIO;
    }

    while (1) {
        read_sensor_and_store(dev, &fs);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
