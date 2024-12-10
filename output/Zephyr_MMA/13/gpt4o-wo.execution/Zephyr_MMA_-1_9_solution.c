
#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_NODE DT_NODELABEL(mma8x5x)
#define NVS_STORAGE_NODE DT_NODELABEL(nvs_storage)

static struct nvs_fs nvs = {
    .sector_size = DT_PROP(NVS_STORAGE_NODE, nvs_sector_size),
    .sector_count = DT_PROP(NVS_STORAGE_NODE, nvs_num_sectors),
    .offset = DT_REG_ADDR(NVS_STORAGE_NODE),
};

void main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8x5x_NODE);
    if (!device_is_ready(dev)) {
        printk("Sensor device not ready\n");
        return;
    }

    int ret = nvs_init(&nvs, DT_REG_ADDR(NVS_STORAGE_NODE));
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return;
    }

    while (1) {
        struct sensor_value accel[3];
        if (sensor_sample_fetch(dev) < 0) {
            printk("Sensor sample fetch failed\n");
            k_sleep(K_SECONDS(5));
            continue;
        }

        if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
            printk("Sensor channel get failed\n");
            k_sleep(K_SECONDS(5));
            continue;
        }

        uint16_t id = 1;
        uint8_t data[6];
        for (int i = 0; i < 3; i++) {
            data[i * 2] = (uint8_t)(accel[i].val1 >> 8);
            data[i * 2 + 1] = (uint8_t)(accel[i].val1 & 0xFF);
        }

        ret = nvs_write(&nvs, id, data, sizeof(data));
        if (ret < 0) {
            printk("NVS write failed: %d\n", ret);
        } else {
            printk("NVS write success: %d bytes\n", ret);
        }

        k_sleep(K_SECONDS(5));
    }
}
