
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

static struct nvs_fs nvs = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = 1,
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

    // Store the sensor data in NVS
    ret = nvs_write(&nvs, 0, accel, sizeof(accel));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Data written to NVS");
    }
}

int main(void)
{
    const struct device *dev;
    int ret;

    // Initialize NVS
    nvs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION_ID);
    ret = nvs_init(&nvs, "storage");
    if (ret) {
        LOG_ERR("Failed to initialize NVS (%d)", ret);
        return -1;
    }

    // Get the MMA8x5x device
    dev = device_get_binding(MMA8x5x_DEVICE_NAME);
    if (!dev) {
        LOG_ERR("Failed to get MMA8x5x device");
        return -1;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
