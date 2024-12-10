
#include <zephyr/init.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/kernel.h>

#define MMA8X5X_NODE DT_INST(0, nxp_mma8x5x)
#define NVS_PARTITION DT_LABEL(DT_INST(0, nvs_partition))

static struct nvs_fs nvs = {
    .sector_size = DT_REG_SIZE(NVS_PARTITION),
    .sector_count = DT_PROP(NVS_PARTITION, nvs_num_sectors),
    .flash_device = NVS_PARTITION,
};

void read_sensor_and_store(const struct device *dev)
{
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret) {
        printk("Sensor fetch failed: %d\n", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        printk("Sensor channel get failed: %d\n", ret);
        return;
    }

    uint8_t data[3];
    data[0] = (uint8_t)accel[0].val1;
    data[1] = (uint8_t)accel[1].val1;
    data[2] = (uint8_t)accel[2].val1;

    ret = nvs_write(&nvs, 0, data, sizeof(data));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    } else {
        printk("NVS write success: %d\n", ret);
    }
}

void main(void)
{
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("Sensor device not ready\n");
        return;
    }

    int ret = nvs_init(&nvs, NVS_PARTITION);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }
}
