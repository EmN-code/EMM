
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_NODELABEL(storage)

static struct nvs_fs nvs = {
    .sector_size = DT_REG_SIZE(NVS_PARTITION),
    .sector_count = DT_PROP(NVS_PARTITION, nvs_num_sectors),
    .offset = DT_REG_ADDR(NVS_PARTITION),
};

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("Device MMA8X5X not ready\n");
        return -1;
    }

    int ret = nvs_init(&nvs, DT_LABEL(NVS_PARTITION));
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return -1;
    }

    while (1) {
        struct sensor_value accel[3];
        ret = sensor_sample_fetch(dev);
        if (ret) {
            printk("Sensor fetch failed: %d\n", ret);
            continue;
        }

        ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (ret) {
            printk("Sensor channel get failed: %d\n", ret);
            continue;
        }

        uint8_t data[3];
        data[0] = (uint8_t)accel[0].val1;
        data[1] = (uint8_t)accel[1].val1;
        data[2] = (uint8_t)accel[2].val1;

        ret = nvs_write(&nvs, 1, data, sizeof(data));
        if (ret < 0) {
            printk("NVS write failed: %d\n", ret);
        } else {
            printk("NVS write success: %d\n", ret);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
