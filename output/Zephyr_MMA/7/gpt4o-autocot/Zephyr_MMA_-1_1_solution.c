
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_LABEL DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION DT_LABEL(DT_ALIAS(storage_partition))

static struct nvs_fs fs;

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];
    int rc;

    if (!device_is_ready(dev)) {
        LOG_ERR("Sensor device %s is not ready", dev->name);
        return;
    }

    rc = sensor_sample_fetch(dev);
    if (rc) {
        LOG_ERR("Failed to fetch sample from %s: %d", dev->name, rc);
        return;
    }

    rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (rc) {
        LOG_ERR("Failed to get channel from %s: %d", dev->name, rc);
        return;
    }

    LOG_INF("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
            accel[0].val1, accel[0].val2,
            accel[1].val1, accel[1].val2,
            accel[2].val1, accel[2].val2);

    // Store the reading value persistently in NVS
    uint8_t data[12];
    memcpy(data, &accel, sizeof(accel));

    rc = nvs_write(&fs, 1, data, sizeof(data));
    if (rc < 0) {
        LOG_ERR("Failed to write to NVS: %d", rc);
    } else {
        LOG_INF("Data written to NVS successfully");
    }
}

int main(void) {
    const struct device *dev = device_get_binding(MMA8x5x_LABEL);
    if (!dev) {
        LOG_ERR("Could not get device binding for MMA8x5x");
        return -1;
    }

    // Initialize NVS
    fs.flash_device = NVS_PARTITION;
    fs.offset = 0;
    fs.sector_size = 4096;
    fs.sector_count = 2;

    int rc = nvs_init(&fs, NVS_PARTITION);
    if (rc) {
        LOG_ERR("Failed to initialize NVS: %d", rc);
        return -1;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
