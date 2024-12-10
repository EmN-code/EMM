// Assuming the Zephyr environment is properly set up on your development machine.

// First, ensure that the environment is set correctly:
// Run in the terminal (replace '/path/to/zephyr' with your actual path):
// export ZEPHYR_BASE='/path/to/zephyr'

#include <zephyr.h>             // Zephyr OS API
#include <zephyr/sys/util.h>    // General utilities
#include <zephyr/sys/slist.h>   // Singly-linked list
#include <zephyr/sys/nvs.h>     // Non-Volatile Storage
#include <device.h>             // For device functions
#include <drivers/sensor.h>     // Sensor interface

#define SENSOR_READ_INTERVAL    K_SECONDS(5) // 5 seconds interval

static const struct device *sensor_device;
static struct nvs_fs fs;

void main(void) {
    int ret;

    // Initialize the NVS file system
    fs.sector_size = 4096;
    fs.sector_count = 3;
    fs.offset = FLASH_AREA_OFFSET(storage);
    ret = nvs_init(&fs, "");
    if (ret) {
        printk("NVS init failed\n");
        return;
    }

    // Fetch the sensor device binding
    sensor_device = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8x5x)));
    if (sensor_device == NULL) {
        printk("Sensor device not found\n");
        return;
    }

    while (1) {
        struct sensor_value val;

        // Trigger sensor reading
        ret = sensor_sample_fetch(sensor_device);
        if (ret) {
            printk("Sensor fetch error\n");
            continue;
        }

        // Get the sensor data
        ret = sensor_channel_get(sensor_device, SENSOR_CHAN_ACCEL_X, &val);
        if (ret) {
            printk("Sensor channel get error\n");
            continue;
        }

        printk("Acceleration (X-axis): %d.%06d\n", val.val1, val.val2);

        // Store the value in NVS
        ret = nvs_write(&fs, 1, &val.val1, sizeof(val.val1));
        if (ret < 0) {
            printk("NVS write error\n");
        }

        // Sleep for defined interval
        k_sleep(SENSOR_READ_INTERVAL);
    }
}