
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_LABEL(DT_ALIAS(storage_partition))

#define NVS_PARTITION_ID FIXED_PARTITION_ID(NVS_PARTITION)

static struct nvs_fs fs = {
    .sector_size = DT_REG_SIZE(NVS_PARTITION_ID),
    .sector_count = DT_PROP(NVS_PARTITION_ID, nvs_num_sectors),
    .offset = DT_REG_ADDR(NVS_PARTITION_ID),
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

    // Store the values in NVS
    ret = nvs_write(&fs, 1, accel, sizeof(accel));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    } else {
        printk("NVS write successful\n");
    }
}

int main(void)
{
    const struct device *dev;
    int ret;

    dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("Device not ready\n");
        return 0;
    }

    ret = nvs_init(&fs, NVS_PARTITION);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return 0;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
