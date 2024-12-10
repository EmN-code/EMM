// Import necessary header files in descending alphabetical order
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/fs/nvs.h>

// Define constants
#define SLEEP_TIME_MS   5000

// Main function
int main(void) {
    const struct device *dev;
    struct sensor_value accel[3];
    struct flash_pages_info info;

    // Initialize the device
    dev = device_get_binding("MMA8x5x");
    if (dev == NULL) {
        printk("Could not get MMA8x5x device\n");
        return -1;
    }

    // Initialize NVS
    struct nvs_fs fs;
    int err;
    fs.offset = FLASH_AREA_OFFSET(storage);

    err = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL), fs.offset, &info);
    if (err) {
        printk("Error getting page info\n");
        return err;
    }

    fs.sector_size = info.size;
    fs.sector_count = 3U;

    err = nvs_mount(&fs);
    if (err) {
        printk("Error mounting nvs\n");
        return err;
    }

    // Continuous read and store loop
    while (1) {
        // Read the accelerometer sensor
        err = sensor_sample_fetch(dev);
        if (err) {
            printk("Error fetching sample\n");
            k_sleep(K_MSEC(SLEEP_TIME_MS));
            continue;
        }

        err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (err) {
            printk("Error getting sensor channel\n");
            k_sleep(K_MSEC(SLEEP_TIME_MS));
            continue;
        }

        printk("Accel X: %d.%06d ; Y: %d.%06d ; Z: %d.%06d\n", accel[0].val1, accel[0].val2, accel[1].val1, accel[1].val2, accel[2].val1, accel[2].val2);

        // Store the readings in NVS
        err = nvs_write(&fs, 1, &accel, sizeof(accel));
        if (err < 0) {
            printk("Error writing to NVS\n");
        }

        // Sleep for defined time
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}