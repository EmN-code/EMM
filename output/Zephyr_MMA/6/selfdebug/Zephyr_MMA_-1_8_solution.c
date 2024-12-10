
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION FIXED_PARTITION_ID(storage_partition)
#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 4

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    fs.flash_device = NVS_PARTITION;
    fs.offset = 0;
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;

    int ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("NVS mount failed: %d", ret);
    } else {
        LOG_INF("NVS mounted successfully");
    }
}

void read_mma8x5x(const struct device *dev) {
    struct sensor_value accel[3];
    int ret = sensor_sample_fetch(dev);
    if (ret) {
        LOG_ERR("Failed to fetch sample from MMA8x5x: %d", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Failed to get sensor data from MMA8x5x: %d", ret);
        return;
    }

    LOG_INF("MMA8x5x: X: %d.%06d, Y: %d.%06d, Z: %d.%06d",
            accel[0].val1, accel[0].val2,
            accel[1].val1, accel[1].val2,
            accel[2].val1, accel[2].val2);

    // Store the read value persistently in NVS
    uint8_t data[12];
    memcpy(data, accel, sizeof(accel));
    ret = nvs_write(&fs, 1, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("NVS write failed: %d", ret);
    } else {
        LOG_INF("NVS write successful");
    }
}

void main(void) {
    const struct device *dev = device_get_binding(MMA8x5x_DEV_NAME);
    if (!dev) {
        LOG_ERR("Failed to get device binding for MMA8x5x");
        return;
    }

    init_nvs();

    while (1) {
        read_mma8x5x(dev);
        k_sleep(K_SECONDS(5));
    }
}
