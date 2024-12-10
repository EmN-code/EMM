
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_DEVICE_NAME "MMA8x5x"
#define NVS_PARTITION_ID 0
#define NVS_PARTITION_LABEL "storage"

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    fs.flash_device = FLASH_AREA_ID(NVS_PARTITION_ID);
    fs.offset = 0;
    fs.sector_size = 4096;
    fs.sector_count = 2;

    int ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("NVS mount failed: %d", ret);
    } else {
        LOG_INF("NVS mounted successfully");
    }
}

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret) {
        LOG_ERR("Sensor sample fetch failed: %d", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Sensor channel get failed: %d", ret);
        return;
    }

    LOG_INF("Acceleration: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
            accel[0].val1, accel[0].val2,
            accel[1].val1, accel[1].val2,
            accel[2].val1, accel[2].val2);

    // Store the read values persistently using NVS
    uint16_t id = 1; // Example ID for the sensor data
    ret = nvs_write(&fs, id, accel, sizeof(accel));
    if (ret < 0) {
        LOG_ERR("NVS write failed: %d", ret);
    } else {
        LOG_INF("NVS write successful");
    }
}

void main(void) {
    const struct device *dev = device_get_binding(MMA8x5x_DEVICE_NAME);
    if (!dev) {
        LOG_ERR("Could not get %s device", MMA8x5x_DEVICE_NAME);
        return;
    }

    init_nvs();

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }
}
