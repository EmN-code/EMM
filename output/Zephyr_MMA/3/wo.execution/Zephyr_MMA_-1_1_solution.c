
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>

#define SLEEP_DURATION_SECONDS 5

void main(void) {
    const struct device *dev;
    struct sensor_value accel[3];

    // Get the device for the MMA8x5x sensor
    dev = device_get_binding("MMA8x5x");
    if (dev == NULL) {
        printk("Device MMA8x5x not found\n");
        return;
    }

    // Check if the device is ready
    if (!device_is_ready(dev)) {
        printk("Device MMA8x5x is not ready\n");
        return;
    }

    while (1) {
        // Fetch the sensor data
        if (sensor_sample_fetch(dev) < 0) {
            printk("Failed to fetch sample\n");
        } else {
            // Read the accelerometer data
            sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);

            // Print the accelerometer data
            printk("Accel X: %d.%06d m/s^2\n", accel[0].val1, accel[0].val2);
            printk("Accel Y: %d.%06d m/s^2\n", accel[1].val1, accel[1].val2);
            printk("Accel Z: %d.%06d m/s^2\n", accel[2].val1, accel[2].val2);
        }

        // Sleep for 5 seconds
        k_sleep(K_SECONDS(SLEEP_DURATION_SECONDS));
    }
}
