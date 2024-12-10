#include <zephyr/kernel.h> // For basic kernel functionalities
#include <zephyr/device.h> // For device binding
#include <zephyr/drivers/sensor.h> // For sensor functionalities
#include <zephyr/drivers/flash.h> // For flash operations
#include <zephyr/fs/nvs.h> // For NVS functionalities

#define SLEEP_TIME_MS 5000 // Sleep time in milliseconds

void main(void) {
    const struct device *mma8x5x_dev, *flash_dev;
    struct sensor_value accel[3]; // Assuming MMA8X5X provides 3-axis accelerometer data
    int ret;

    // Bind the sensor device
    mma8x5x_dev = device_get_binding("MMA8X5X");
    if (!mma8x5x_dev) {
        printk("No device found\n");
        return;
    }

    // Bind the flash storage device
    flash_dev = device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
    if (!flash_dev) {
        printk("No flash device found\n");
        return;
    }

    // Initialize NVS
    struct nvs_fs nvs;
    nvs.sector_size = FLASH_SECTOR_SIZE;
    nvs.sector_count = 3;
    nvs.offset = FLASH_AREA_OFFSET(storage);
    ret = nvs_init(&nvs, flash_dev->driver_data);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
    }

    while (1) {
        // Fetch sensor data
        ret = sensor_sample_fetch(mma8x5x_dev);
        if (ret) {
            printk("Failed to fetch sensor sample: %d\n", ret);
            k_sleep(K_MSEC(SLEEP_TIME_MS));
            continue; // Retry fetching sample
        }

        // Get sensor data
        ret = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (ret) {
            printk("Failed to get sensor channel: %d\n", ret);
            k_sleep(K_MSEC(SLEEP_TIME_MS));
            continue; // Retry getting data
        }

        // Store data in NVS
        ret = nvs_write(&nvs, 1, &accel, sizeof(accel));
        if (ret < 0) {
            printk("Failed to write to NVS: %d\n", ret);
        } else {
            printk("Stored accel data in NVS\n");
        }

        // Sleep for defined period
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}