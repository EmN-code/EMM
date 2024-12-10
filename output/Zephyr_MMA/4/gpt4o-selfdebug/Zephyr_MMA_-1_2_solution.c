#include <zephyr.h>
#include <storage/flash_map.h>
#include <storage/flash.h>
#include <drivers/sensor.h>
#include <device.h>

#define SLEEP_TIME_MS   5000

void main(void) {
    const struct device *dev = device_get_binding("MMA8x5x");
    struct sensor_value accel_value;

    if (dev == NULL) {
        printk("No device found\n");
        return;
    }

    while (1) {
        sensor_sample_fetch(dev);
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &accel_value);

        // Store value in NVS
        // Assuming you have NVS setup, here you'd typically call nvs_write
        // Example: nvs_write(&fs, whatever_id, &accel_value, sizeof(accel_value));

        printk("Acceleration X: %d\n", accel_value.val1);
        k_msleep(SLEEP_TIME_MS);
    }
}