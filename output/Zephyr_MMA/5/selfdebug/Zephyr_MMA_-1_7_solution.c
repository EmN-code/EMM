
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
        LOG_ERR("Failed to fetch sample from %s", MMA8x5x_DEVICE_NAME);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Failed to get channel data from %s", MMA8x5x_DEVICE_NAME);
        return;
    }

    uint8_t data[3];
    data[0] = (uint8_t)accel[0].val1;
    data[1] = (uint8_t)accel[1].val1;
    data[2] = (uint8_t)accel[2].val1;

    ret = nvs_write(fs, 0, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write data to NVS: %d", ret);
    } else {
        LOG_INF("Data written to NVS: %d, %d, %d", data[0], data[1], data[2]);
    }
}

int main(void)
{
    const struct device *dev = device_get_binding(MMA8x5x_DEVICE_NAME);
    if (!dev) {
        LOG_ERR("Could not get %s device", MMA8x5x_DEVICE_NAME);
        return -1;
    }

    struct nvs_fs fs;
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = 1;
    fs.offset = FIXED_PARTITION_ID(storage_partition);

    int ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("Failed to mount NVS: %d", ret);
        return -1;
    }

    while (1) {
        read_sensor_and_store(dev, &fs);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
