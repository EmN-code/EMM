
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_NODELABEL(storage_partition)

LOG_MODULE_REGISTER(main);

struct nvs_fs nvs = {
    .flash_device = (void *)DT_MTD_FROM_FIXED_PARTITION(NVS_PARTITION),
    .offset = DT_REG_ADDR(NVS_PARTITION),
    .sector_size = DT_REG_SIZE(NVS_PARTITION),
    .sector_count = 3,
};

void main(void)
{
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    struct sensor_value val;
    int ret;

    if (!device_is_ready(dev)) {
        LOG_ERR("Device %s is not ready", dev->name);
        return;
    }

    ret = nvs_init(&nvs, "storage");
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return;
    }

    while (1) {
        ret = sensor_sample_fetch(dev);
        if (ret) {
            LOG_ERR("Sensor fetch failed: %d", ret);
            return;
        }

        ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &val);
        if (ret) {
            LOG_ERR("Sensor channel get failed: %d", ret);
            return;
        }

        ret = nvs_write(&nvs, 1, &val, sizeof(val));
        if (ret != sizeof(val)) {
            LOG_ERR("NVS write failed: %d", ret);
            return;
        }

        LOG_INF("Acceleration: x=%d.%06d, y=%d.%06d, z=%d.%06d",
                val.val1, val.val2, val.val1, val.val2, val.val1, val.val2);

        k_sleep(K_SECONDS(5));
    }
}
