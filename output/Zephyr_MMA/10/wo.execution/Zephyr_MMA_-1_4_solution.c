
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>

LOG_MODULE_REGISTER(main);

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define STORAGE_NODE DT_NODELABEL(storage)

#define NVS_SECTOR_SIZE DT_REG_SIZE(DT_NODELABEL(storage), 0)
#define NVS_NUM_SECTORS DT_PROP(DT_NODELABEL(storage), nvs_num_sectors)
#define NVS_ADDRESS DT_REG_ADDR(DT_NODELABEL(storage), 0)

static struct nvs_fs fs = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = NVS_NUM_SECTORS,
    .flash_device = (const struct device *)DEVICE_DT_GET(STORAGE_NODE),
    .offset = NVS_ADDRESS,
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

    uint8_t data[3];
    data[0] = (uint8_t)accel[0].val1;
    data[1] = (uint8_t)accel[1].val1;
    data[2] = (uint8_t)accel[2].val1;

    ret = nvs_write(&fs, 1, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Data written to NVS");
    }
}

int main(void)
{
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s is not ready", dev->name);
        return -ENODEV;
    }

    int ret = nvs_init(&fs, fs.flash_device);
    if (ret) {
        LOG_ERR("Failed to init NVS (%d)", ret);
        return -EIO;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
