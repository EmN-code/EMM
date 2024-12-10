
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_DEVICE_NAME "MMA8x5x"
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_SECTOR_SIZE 4096
#define NVS_NUM_SECTORS 4

LOG_MODULE_REGISTER(main);

static struct nvs_fs nvs_storage = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = NVS_NUM_SECTORS,
    .offset = FIXED_PARTITION_OFFSET(storage_partition),
};

void read_sensor_and_store(const struct device *dev)
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

    LOG_INF("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
            accel[0].val1, accel[0].val2,
            accel[1].val1, accel[1].val2,
            accel[2].val1, accel[2].val2);

    // Store the read value in NVS
    ret = nvs_write(&nvs_storage, 1, accel, sizeof(accel));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Data written to NVS");
    }
}

void main(void)
{
    const struct device *dev;
    int ret;

    dev = device_get_binding(MMA8x5x_DEVICE_NAME);
    if (!dev) {
        LOG_ERR("Could not get %s device", MMA8x5x_DEVICE_NAME);
        return;
    }

    // Initialize NVS
    nvs_storage.flash_device = FIXED_PARTITION_DEVICE(storage_partition);
    ret = nvs_init(&nvs_storage, NVS_PARTITION_ID);
    if (ret) {
        LOG_ERR("Failed to initialize NVS (%d)", ret);
        return;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }
}
