
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/sys/util.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define STORAGE_NODE DT_NODELABEL(storage)

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs = {
    .sector_size = DT_REG_SIZE(STORAGE_NODE),
    .sector_count = DT_PROP(STORAGE_NODE, erase_block_size),
    .offset = DT_REG_ADDR(STORAGE_NODE),
};

static const struct device *mma8x5x_dev = DEVICE_DT_GET(MMA8X5X_NODE);

int main(void) {
    int ret;
    struct sensor_value accel[3];
    uint8_t data[6];

    if (!device_is_ready(mma8x5x_dev)) {
        LOG_ERR("MMA8X5X device not ready");
        return -1;
    }

    ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("NVS mount failed: %d", ret);
        return -1;
    }

    while (1) {
        ret = sensor_sample_fetch(mma8x5x_dev);
        if (ret) {
            LOG_ERR("Sensor fetch failed: %d", ret);
            continue;
        }

        ret = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (ret) {
            LOG_ERR("Sensor channel get failed: %d", ret);
            continue;
        }

        data[0] = (uint8_t)(accel[0].val1 >> 8);
        data[1] = (uint8_t)(accel[0].val1 & 0xFF);
        data[2] = (uint8_t)(accel[1].val1 >> 8);
        data[3] = (uint8_t)(accel[1].val1 & 0xFF);
        data[4] = (uint8_t)(accel[2].val1 >> 8);
        data[5] = (uint8_t)(accel[2].val1 & 0xFF);

        ret = nvs_write(&fs, 0, data, sizeof(data));
        if (ret < 0) {
            LOG_ERR("NVS write failed: %d", ret);
        } else {
            LOG_INF("NVS write success: %d bytes", ret);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
