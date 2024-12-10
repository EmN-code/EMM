
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define NVS_PARTITION_LABEL "storage"
#define SENSOR_NODE DT_ALIAS(mma8x5x)

static struct nvs_fs nvs = {
    .sector_size = DT_REG_SIZE(DT_ALIAS(storage_PARTITION_ID)),
    .sector_count = DT_REG_SIZE(DT_ALIAS(storage_PARTITION_ID)) / DT_REG_SIZE(DT_ALIAS(storage_PARTITION_ID)),
    .offset = DT_REG_ADDR(DT_ALIAS(storage_PARTITION_ID)),
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

    ret = nvs_write(&nvs, 0, &accel, sizeof(accel));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Stored sensor data in NVS");
    }
}

int main(void)
{
    const struct device *dev;
    int ret;

    dev = device_get_binding(DT_LABEL(SENSOR_NODE));
    if (!dev) {
        LOG_ERR("Failed to get device binding");
        return -1;
    }

    ret = nvs_init(&nvs, NVS_PARTITION_LABEL);
    if (ret) {
        LOG_ERR("Failed to initialize NVS (%d)", ret);
        return -1;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
