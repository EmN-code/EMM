#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/flash_map.h>

#define SLEEP_TIME_MS   5000

void main(void) {
    const struct device *dev_sensor;
    const struct device *dev_flash;
    struct sensor_value sensor_value;

    // Initialize the sensor MMA8x5x
    dev_sensor = device_get_binding("MMA8X5X");

    if (!dev_sensor) {
        printk("No sensor to find: MMA8X5X\n");
        return;
    }

    // Initialize the flash device (for NVS)
    dev_flash = device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);

    if (!dev_flash) {
        printk("Flash device not found\n");
        return;
    }

    // Insert NVS configuration and logic here as per your actual setup
    // This is a placeholder to indicate where that logic would go

    while (1) {
        // Read sensor value
        if (sensor_sample_fetch(dev_sensor) < 0) {
            printk("Sensor read error\n");
        } else {
            sensor_channel_get(dev_sensor, SENSOR_CHAN_ACCEL_XYZ, &sensor_value);
            printk("Sensor Value X: %d\n", sensor_value.val1);

            // Persistence logic goes here (using NVS)
            // Example: write the value to NVS
            // Placeholder for NVS write operation
        }

        // Sleep for the specified time
        k_msleep(SLEEP_TIME_MS);
    }
}
