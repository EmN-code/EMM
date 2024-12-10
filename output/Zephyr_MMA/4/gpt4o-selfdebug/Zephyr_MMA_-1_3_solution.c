#include <zephyr/sys/util.h>   // Zephyr utility functions
#include <zephyr/sys/mutex.h>  // Zephyr mutex for synchronization
#include <zephyr/syscalls/i2c.h>  // Zephyr I2C support
#include <zephyr/device.h>  // Device API for accessing hardware
#include <zephyr/drivers/sensor.h>  // Sensor driver access
#include <zephyr/settings/nvs.h>  // Non-Volatile Storage
#include <zephyr/kernel.h>  // Kernel support

// Main function to read sensor data and store it persistently
int main(void) {
    const struct device *i2c_dev = device_get_binding("I2C_0");
    const struct device *sensor_dev = device_get_binding("MMA8X5X");

    // Check if devices are ready
    if (!device_is_ready(i2c_dev) || !device_is_ready(sensor_dev)) {
        return -1;  // Exit if devices are not ready
    }

    struct sensor_value accel[3];
    struct nvs_fs fs;
    fs.offset = FLASH_AREA_OFFSET(storage);
    fs.sector_size = FLASH_AREA_SIZE(storage);

    // Initialize Non-Volatile Storage
    nvs_init(&fs, "storage");

    while (1) {
        // Fetch sensor data
        sensor_sample_fetch(sensor_dev);
        sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);

        // Log and store the data using NVS
        printk("Accel X: %d, Y: %d, Z: %d\n", accel[0].val1, accel[1].val1, accel[2].val1);

        nvs_write(&fs, 1, &accel, sizeof(accel));

        // Sleep for 5 seconds
        k_sleep(K_SECONDS(5));
    }

    return 0;
}