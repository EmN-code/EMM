
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>

#define MMA8X5X_LABEL DT_LABEL(DT_INST(0, nxp_mma8x5x))
#define NVS_PARTITION DT_LABEL(DT_ALIAS(storage))

static struct nvs_fs fs = {
    .sector_size = DT_REG_SIZE(DT_NODELABEL(storage)),
    .sector_count = DT_PROP(DT_NODELABEL(storage), size) / DT_REG_SIZE(DT_NODELABEL(storage)),
    .offset = DT_REG_ADDR(DT_NODELABEL(storage)),
};

void read_sensor_and_store(const struct device *dev) {
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

    printk("Accel: X=%d.%06d Y=%d.%06d Z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    ret = nvs_write(&fs, 1, accel, sizeof(accel));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    } else {
        printk("NVS write successful\n");
    }
}

int main(void) {
    const struct device *dev;
    int ret;

    dev = device_get_binding(MMA8X5X_LABEL);
    if (!dev) {
        printk("Could not get MMA8X5X device\n");
        return -1;
    }

    ret = nvs_mount(&fs);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
        return -1;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
