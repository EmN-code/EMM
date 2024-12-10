
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_DEVICE_NODE DT_NODELABEL(mma8x5x)
#define MMA8x5x_DEVICE DEVICE_DT_GET(MMA8x5x_DEVICE_NODE)

#define NVS_PARTITION_NODE DT_NODELABEL(storage)
#define NVS_PARTITION DT_REG_ADDR(NVS_PARTITION_NODE)
#define NVS_SIZE DT_REG_SIZE(NVS_PARTITION_NODE)

struct nvs_fs fs = {
    .sector_size = DT_PROP(NVS_PARTITION_NODE, erase_block_size),
    .sector_count = DT_PROP(NVS_PARTITION_NODE, size) / DT_PROP(NVS_PARTITION_NODE, erase_block_size),
    .offset = NVS_PARTITION,
};

void read_sensor_and_store(const struct device *dev) {
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

    ret = nvs_write(&fs, 0, &accel, sizeof(accel));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Stored sensor data in NVS");
    }
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8x5x_DEVICE_NODE);
    int ret;

    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s is not ready", dev->name);
        return 0;
    }

    ret = nvs_init(&fs, DT_REG_ADDR(NVS_PARTITION_NODE));
    if (ret) {
        LOG_ERR("Failed to init NVS (%d)", ret);
        return 0;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
