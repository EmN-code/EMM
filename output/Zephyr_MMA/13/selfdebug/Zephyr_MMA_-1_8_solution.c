
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/nvs.h>
#include <zephyr/sys/printk.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define NVS_NODE DT_NODELABEL(nvs)

#define NVS_PARTITION_ID 0
#define NVS_DATA_ID 1

static struct nvs_fs nvs_config = {
    .sector_size = DT_REG_SIZE(NVS_NODE),
    .sector_count = DT_PROP(NVS_NODE, erase_block_size),
    .offset = DT_REG_ADDR(NVS_NODE),
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

    printk("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    // Store the sensor data in NVS
    ret = nvs_write(&nvs_config, NVS_DATA_ID, accel, sizeof(accel));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    } else {
        printk("NVS write successful\n");
    }
}

int main(void)
{
    const struct device *mma8x5x_dev;
    int ret;

    mma8x5x_dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(mma8x5x_dev)) {
        printk("MMA8X5X device not ready\n");
        return -ENODEV;
    }

    ret = nvs_init(&nvs_config, DT_LABEL(NVS_NODE));
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return -EIO;
    }

    while (1) {
        read_sensor_and_store(mma8x5x_dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
