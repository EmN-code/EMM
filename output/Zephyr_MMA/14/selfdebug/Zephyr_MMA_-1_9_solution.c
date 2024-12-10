
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define FLASH_NODE DT_NODELABEL(flash_controller)

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_PARTITION FIXED_PARTITION_DEVICE(storage_partition)

static struct nvs_fs fs;

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];
    int rc;

    rc = sensor_sample_fetch(dev);
    if (rc) {
        printk("Sensor fetch failed: %d\n", rc);
        return;
    }

    rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (rc) {
        printk("Sensor channel get failed: %d\n", rc);
        return;
    }

    printk("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    // Store the read value persistently in NVS
    uint8_t data[3];
    data[0] = (uint8_t)accel[0].val1;
    data[1] = (uint8_t)accel[1].val1;
    data[2] = (uint8_t)accel[2].val1;

    rc = nvs_write(&fs, 1, data, sizeof(data));
    if (rc < 0) {
        printk("NVS write failed: %d\n", rc);
    } else {
        printk("NVS write success: %d bytes\n", rc);
    }
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("MMA8X5X device not ready\n");
        return -ENODEV;
    }

    // Initialize NVS
    fs.flash_device = NVS_PARTITION;
    fs.offset = FIXED_PARTITION_OFFSET(storage_partition);
    fs.sector_size = 4096;
    fs.sector_count = 2;

    int rc = nvs_init(&fs, "storage");
    if (rc) {
        printk("NVS init failed: %d\n", rc);
        return -EIO;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
