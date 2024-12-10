
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION FIXED_PARTITION_ID(storage_partition)

static struct nvs_fs fs = {
    .sector_size = DT_SIZE_K(4),
    .sector_count = 2,
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
    ret = nvs_write(&fs, 1, accel, sizeof(accel));
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

    dev = device_get_binding(MMA8x5x_DEV_NAME);
    if (!dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return;
    }

    ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("Failed to mount NVS (%d)", ret);
        return;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }
}
