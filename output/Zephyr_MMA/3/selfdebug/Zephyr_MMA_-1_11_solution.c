
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define NVS_PARTITION DT_NODELABEL(storage)

#define NVS_SECTOR_SIZE 4096
#define NVS_NUM_SECTORS 4

static struct nvs_fs nvs = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = NVS_NUM_SECTORS,
    .flash_device = DEVICE_DT_GET(NVS_PARTITION),
};

void read_sensor_and_store(const struct device *dev)
{
    struct sensor_value accel[3];
    int rc;

    rc = sensor_sample_fetch(dev);
    if (rc) {
        printk("Sensor fetch failed (err %d)\n", rc);
        return;
    }

    rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (rc) {
        printk("Sensor get failed (err %d)\n", rc);
        return;
    }

    printk("Accel: x=%d.%06d, y=%d.%06d, z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    // Store the sensor data in NVS
    uint8_t data[3 * sizeof(struct sensor_value)];
    memcpy(data, accel, sizeof(data));

    rc = nvs_write(&nvs, 0, data, sizeof(data));
    if (rc < 0) {
        printk("NVS write failed (err %d)\n", rc);
    } else {
        printk("NVS write successful\n");
    }
}

int main(void)
{
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("MMA8X5X device not ready\n");
        return -ENODEV;
    }

    int rc = nvs_init(&nvs, DT_LABEL(NVS_PARTITION));
    if (rc) {
        printk("NVS init failed (err %d)\n", rc);
        return -EIO;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
