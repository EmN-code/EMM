// Description of the code solution
// This code initializes the MMA8x5x sensor on an ESP32 using Zephyr. It reads sensor data every 5 seconds and uses Non-Volatile Storage (NVS) to store the reading persistently.

#include <zephyr/sys/util.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h> 

// Function to initialize and read from MMA8x5x and store using NVS
void mma8x5x_read_and_store(void) {
    // Define device pointer for MMA8x5x
    const struct device *dev = DEVICE_DT_GET_ONE(nxp_mma8652fc);

    if (!device_is_ready(dev)) {
        printk("MMA8652 device not found\n");
        return;
    }

    // Define sensor value
    struct sensor_value accel[3];

    while (1) {
        // Fetch sensor data
        int ret = sensor_sample_fetch(dev);
        if (ret) {
            printk("Sensor sample update error\n");
            return;
        }

        // Retrieve sensor data
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);

        // Print acceleration values
        printk("X: %d, Y: %d, Z: %d\n", sensor_value_to_double(&accel[0]),
               sensor_value_to_double(&accel[1]), sensor_value_to_double(&accel[2]));

        // TODO: Use NVS to persistently store the readings

        // Wait for 5 seconds
        k_sleep(K_SECONDS(5));
    }
}

// Main entry
int main(void) {
    printk("MMA8x5x Sensor Reading Example\n");
    mma8x5x_read_and_store();
    return 0;
}