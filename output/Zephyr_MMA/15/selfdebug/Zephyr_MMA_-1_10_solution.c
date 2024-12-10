
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

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

    ret = nvs_write(fs, 0, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Data written to NVS");
    }
}

int main(void) {
    const struct device *mma8x5x_dev;
    struct nvs_fs fs;
    int ret;

    mma8x5x_dev = device_get_binding(MMA8x5x_DEV_NAME);
    if (!mma8x5x_dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -1;
    }

    fs.sector_size = DT_REG_SIZE(NVS_PARTITION_ID);
    fs.sector_count = 1;
    fs.offset = DT_REG_ADDR(NVS_PARTITION_ID);

    ret = nvs_init(&fs, DT_LABEL(NVS_PARTITION_ID));
    if (ret) {
        LOG_ERR("Failed to init NVS (%d)", ret);
        return -1;
    }

    while (1) {
        read_sensor_and_store(mma8x5x_dev, &fs);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
