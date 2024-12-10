
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8X5X_NODE DT_INST(0, nxp_mma8x5x)
#define STORAGE_PARTITION DT_NODELABEL(storage_partition)

static struct nvs_fs fs = {
    .sector_size = DT_REG_SIZE(STORAGE_PARTITION),
    .sector_count = DT_REG_SIZE(STORAGE_PARTITION) / DT_REG_SIZE(STORAGE_PARTITION),
    .offset = DT_REG_ADDR(STORAGE_PARTITION),
};

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];
    int ret;

    if (sensor_sample_fetch(dev) < 0) {
        printk("Sensor fetch failed\n");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        printk("Failed to get sensor data\n");
        return;
    }

    printk("Accel: X=%d.%06d Y=%d.%06d Z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    ret = nvs_write(&fs, 1, &accel, sizeof(accel));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    } else {
        printk("NVS write success: %d bytes\n", ret);
    }
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    int ret;

    if (!device_is_ready(dev)) {
        printk("Sensor device not ready\n");
        return -1;
    }

    ret = nvs_init(&fs, DT_REG_ADDR(STORAGE_PARTITION));
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return -1;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
