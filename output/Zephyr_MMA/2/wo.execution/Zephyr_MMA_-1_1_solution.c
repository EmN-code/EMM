
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>

#define SLEEP_DURATION_SECONDS 5

void main(void) {
    const struct device *dev;
    struct sensor_value accel[3];

    // Get the device instance for MMA8x5x
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
        // Read the accelerometer data
        if (sensor_sample_fetch(dev) < 0) {
            printk("Failed to fetch sample from MMA8x5x\n");
        } else {
            sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
            printk("Accelerometer (m/s^2): X=%f, Y=%f, Z=%f\n",
                   sensor_value_to_double(&accel[0]),
                   sensor_value_to_double(&accel[1]),
                   sensor_value_to_double(&accel[2]));
        }

        // Sleep for 5 seconds
        k_sleep(K_SECONDS(SLEEP_DURATION_SECONDS));
    }
}
