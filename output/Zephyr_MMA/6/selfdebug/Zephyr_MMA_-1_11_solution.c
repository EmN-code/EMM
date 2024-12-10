
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_NODELABEL(storage_partition)

#if !DT_NODE_HAS_STATUS(MMA8x5x_NODE, okay)
#error "MMA8x5x device tree node is not defined or enabled"
#endif

#if !DT_NODE_HAS_STATUS(NVS_PARTITION, okay)
#error "NVS partition device tree node is not defined or enabled"
#endif

const struct device *mma8x5x_dev = DEVICE_DT_GET(MMA8x5x_NODE);
const struct device *nvs_dev;

static struct nvs_fs nvs = {
    .sector_size = DT_REG_SIZE(NVS_PARTITION),
    .sector_count = DT_PROP(NVS_PARTITION, size) / DT_REG_SIZE(NVS_PARTITION),
    .offset = DT_REG_ADDR(NVS_PARTITION),
};

void read_sensor_and_store(void)
{
    struct sensor_value accel[3];
    int rc;

    if (!device_is_ready(mma8x5x_dev)) {
        LOG_ERR("MMA8x5x device is not ready");
        return;
    }

    rc = sensor_sample_fetch(mma8x5x_dev);
    if (rc) {
        LOG_ERR("Failed to fetch sample from MMA8x5x (err %d)", rc);
        return;
    }

    rc = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (rc) {
        LOG_ERR("Failed to get sensor data (err %d)", rc);
        return;
    }

    LOG_INF("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d",
            accel[0].val1, accel[0].val2,
            accel[1].val1, accel[1].val2,
            accel[2].val1, accel[2].val2);

    // Store the sensor data in NVS
    uint8_t data[12];
    memcpy(data, accel, sizeof(accel));

    rc = nvs_write(&nvs, 1, data, sizeof(data));
    if (rc < 0) {
        LOG_ERR("Failed to write to NVS (err %d)", rc);
    } else {
        LOG_INF("Data written to NVS");
    }
}

int main(void)
{
    int rc;

    // Initialize NVS
    nvs_dev = device_get_binding(DT_LABEL(NVS_PARTITION));
    if (!nvs_dev) {
        LOG_ERR("Failed to get NVS device");
        return -ENODEV;
    }

    rc = nvs_init(&nvs, DT_LABEL(NVS_PARTITION));
    if (rc) {
        LOG_ERR("Failed to initialize NVS (err %d)", rc);
        return rc;
    }

    while (1) {
        read_sensor_and_store();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
