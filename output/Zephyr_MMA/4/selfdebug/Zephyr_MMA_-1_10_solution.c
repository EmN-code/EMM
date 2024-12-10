
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define FLASH_NODE DT_NODELABEL(flash_controller)
#define NVS_PARTITION DT_NODELABEL(storage_partition)

#define NVS_SECTOR_COUNT 4
#define NVS_SECTOR_SIZE 4096

struct nvs_fs fs;

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];
    int rc;

    if (!device_is_ready(dev)) {
        printk("Sensor device not ready\n");
        return;
    }

    rc = sensor_sample_fetch(dev);
    if (rc) {
        printk("Failed to fetch sample (%d)\n", rc);
        return;
    }

    rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (rc) {
        printk("Failed to get channel (%d)\n", rc);
        return;
    }

    printk("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    // Store the read values in NVS
    uint8_t data[12];
    memcpy(data, accel, sizeof(accel));

    rc = nvs_write(&fs, 1, data, sizeof(data));
    if (rc < 0) {
        printk("Failed to write to NVS (%d)\n", rc);
    } else {
        printk("Data written to NVS\n");
    }
}

int main(void) {
    const struct device *mma8x5x_dev = DEVICE_DT_GET(MMA8X5X_NODE);
    const struct device *flash_dev = DEVICE_DT_GET(FLASH_NODE);

    if (!device_is_ready(mma8x5x_dev)) {
        printk("MMA8X5X device not ready\n");
        return -ENODEV;
    }

    if (!device_is_ready(flash_dev)) {
        printk("Flash device not ready\n");
        return -ENODEV;
    }

    // Initialize NVS
    fs.flash_device = flash_dev;
    fs.offset = DT_REG_ADDR(NVS_PARTITION);
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;

    int rc = nvs_init(&fs, "storage");
    if (rc) {
        printk("Failed to init NVS (%d)\n", rc);
        return -EIO;
    }

    while (1) {
        read_sensor_and_store(mma8x5x_dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
