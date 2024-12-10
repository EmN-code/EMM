#include <zephyr/types.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/flash_data.h>

#define STORAGE_PARTITION 0
#define STORAGE_LABEL DT_LABEL(DT_NODELABEL(storage0))
#define READ_INTERVAL 5000

void read_sensor_and_store() {
    const struct device *dev = device_get_binding("MMA8X5X");
    if (!dev) {
        printk("Failed to get sensor device binding\n");
        return;
    }

    struct sensor_value accel[3];
    int ret = sensor_sample_fetch(dev);
    if (ret != 0) {
        printk("Sensor sample fetch error\n");
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret != 0) {
        printk("Sensor channel get error\n");
        return;
    }

    printk("Accel X: %d, Y: %d, Z: %d\n", accel[0].val1, accel[1].val1, accel[2].val1);

    // Store in NVS (Non-Volatile Storage)
    const struct flash_area *fa;
    ret = flash_area_open(STORAGE_PARTITION, &fa);
    if (ret != 0) {
        printk("Flash area open error\n");
        return;
    }

    uint32_t data[3] = {accel[0].val1, accel[1].val1, accel[2].val1};
    ret = flash_area_write(fa, 0, data, sizeof(data));
    if (ret != 0) {
        printk("Flash write error\n");
        return;
    }

    printk("Data stored persistently\n");
}

void main(void) {
    printk("Starting MMA8x5x sensor reading\n");

    // Setup a timer that triggers every READ_INTERVAL ms
    while (1) {
        read_sensor_and_store();
        k_msleep(READ_INTERVAL);
    }
}