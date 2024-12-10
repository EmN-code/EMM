// Description: This code reads sensor data every 5 seconds and stores it using NVS on an ESP32 board using Zephyr.

// Import necessary header files
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>

#define SLEEP_TIME_MS   5000

/* NVS file system configuration */
static struct nvs_fs fs;
#define STORAGE_PARTITION_OFFSET 0xf0000
#define STORAGE_PARTITION_SIZE   0x1000

void main(void) {
    int rc;
    struct device *dev;
    struct sensor_value accel[3];

    // Set up NVS
    fs.offset = STORAGE_PARTITION_OFFSET;
    rc = nvs_init(&fs, STORAGE_PARTITION_SIZE);
    if (rc) {
        printk("NVS init failed: %d\n", rc);
        return;
    }

    dev = device_get_binding("MMA8X5X");
    if (dev == NULL) {
        printk("Failed to get MMA8X5X device\n");
        return;
    }
    printk("Device is %p, name is %s\n", dev, dev->config->name);

    while (1) {
        rc = sensor_sample_fetch(dev);
        if (rc) {
            printk("Sensor sample update error\n");
            return;
        }

        rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc) {
            printk("Cannot read MMA8X5X channels\n");
            return;
        }

        printk("Acceleration (m/s^2): x = %f, y = %f, z = %f\n",
               sensor_value_to_double(&accel[0]),
               sensor_value_to_double(&accel[1]),
               sensor_value_to_double(&accel[2]));

        /* Store the readings persistently using NVS */
        // For simplicity let's just store the X-axis value
        rc = nvs_write(&fs, 1, &accel[0], sizeof(accel[0]));
        if (rc < 0) {
            printk("Failed to write in NVS: %d\n", rc);
        }

        k_msleep(SLEEP_TIME_MS);
    }
}