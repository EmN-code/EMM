#include <zephyr/types.h> // Include for standard types
#include <zephyr/sys/util.h> // Include for utilities
#include <zephyr/sys/slist.h> // Include for singly linked list
#include <zephyr/kernel.h> // Include for kernel specific functions
#include <zephyr/sys/mutex.h> // Include for mutex
#include <device.h> // Include for device driver API
#include <drivers/sensor.h> // Include for sensor API
#include <nvs/nvs.h> // Include for NVS API

#define SLEEP_TIME_MS 5000 // Sleep time between reads

void main(void)
{
    const struct device *dev_mma8x5x;
    struct sensor_value sensor_data;
    struct nvs_fs nvs;
    int rc;

    // Initialize the NVS file system structure
    nvs.offset = FLASH_AREA_OFFSET(storage);
    nvs.sector_size = FLASH_AREA_SIZE(storage);

    // Mount nvs file system
    rc = nvs_init(&nvs, "storage");
    if (rc) {
        printk("Flash Init failed: %d\n", rc);
        return;
    }

    dev_mma8x5x = device_get_binding("MMA8x5x");
    if (dev_mma8x5x == NULL) {
        printk("No device found \n");
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(dev_mma8x5x);
        if (rc) {
            printk("Failed to fetch data sample: %d\n", rc);
            continue;
        }

        rc = sensor_channel_get(dev_mma8x5x, SENSOR_CHAN_ACCEL_XYZ, &sensor_data);
        if (rc) {
            printk("Failed to get data channel: %d\n", rc);
            continue;
        }

        printk("Accel: %d.%06d\n", sensor_data.val1, sensor_data.val2);
        
        // Store the sensor reading in NVS
        rc = nvs_write(&nvs, 1, &sensor_data, sizeof(sensor_data));
        if (rc < 0) {
            printk("Failed to store sensor data: %d\n", rc);
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}
