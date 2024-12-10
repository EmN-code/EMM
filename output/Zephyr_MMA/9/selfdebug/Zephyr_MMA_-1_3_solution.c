
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION DT_LABEL(DT_ALIAS(storage_partition))

LOG_MODULE_REGISTER(main);

void read_sensor_and_store(const struct device *dev, struct nvs_fs *fs) {
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

    uint8_t data[3 * sizeof(struct sensor_value)];
    memcpy(data, accel, sizeof(data));

    ret = nvs_write(fs, 1, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Data written to NVS");
    }
}

int main(void) {
    const struct device *dev;
    struct nvs_fs fs;
    int ret;

    dev = device_get_binding(MMA8x5x_DEV_NAME);
    if (!dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -1;
    }

    fs.flash_device = device_get_binding(NVS_PARTITION);
    if (!fs.flash_device) {
        LOG_ERR("Could not get NVS partition device");
        return -1;
    }

    fs.offset = FIXED_PARTITION_OFFSET(storage_partition);
    fs.sector_size = 4096;
    fs.sector_count = 2;

    ret = nvs_init(&fs, NVS_PARTITION);
    if (ret) {
        LOG_ERR("Failed to initialize NVS (%d)", ret);
        return -1;
    }

    while (1) {
        read_sensor_and_store(dev, &fs);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
