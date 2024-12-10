
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define STORAGE_NODE DT_NODELABEL(storage)

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)
#define NVS_PARTITION DT_LABEL(STORAGE_NODE)

static struct nvs_fs fs = {
    .sector_size = DT_REG_SIZE(STORAGE_NODE),
    .sector_count = DT_REG_SIZE(STORAGE_NODE) / DT_REG_SIZE(STORAGE_NODE),
    .offset = DT_REG_ADDR(STORAGE_NODE),
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

    uint8_t data[6];
    data[0] = (uint8_t)(accel[0].val1 >> 8);
    data[1] = (uint8_t)(accel[0].val1 & 0xFF);
    data[2] = (uint8_t)(accel[1].val1 >> 8);
    data[3] = (uint8_t)(accel[1].val1 & 0xFF);
    data[4] = (uint8_t)(accel[2].val1 >> 8);
    data[5] = (uint8_t)(accel[2].val1 & 0xFF);

    ret = nvs_write(&fs, 0, data, sizeof(data));
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

    int ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("Failed to mount NVS (%d)", ret);
        return -EIO;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
