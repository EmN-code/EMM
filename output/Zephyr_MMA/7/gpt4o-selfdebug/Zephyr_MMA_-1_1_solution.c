#include <zephyr.h>
#include <drivers/sensor.h>
#include <device.h>
#include <nvs/nvs.h>
#include <sys/printk.h>

#define SLEEP_TIME_MS 5000

void main(void) {
    const struct device *mma8x5x_dev;
    struct sensor_value accel_value;
    const struct device *nvs_dev;
    struct nvs_fs nvs;
    int rc;

    // Initialization of NVS
    nvs.flash_id = DT_FLASH_AREA_STORAGE_ID;
    nvs.offset = DT_FLASH_AREA_STORAGE_OFFSET;
    nvs.sector_size = DT_FLASH_AREA_STORAGE_SIZE;
    nvs.sector_count = DT_FLASH_AREA_STORAGE_SIZE / nvs.sector_size;

    rc = nvs_init(&nvs, nvs_dev->mvcc);
    if (rc) {
        printk("NVS initialization failed\n");
        return;
    }

    // Get the MMA8x5x sensor device
    mma8x5x_dev = device_get_binding("MMA8X5X");
    if (!mma8x5x_dev) {
        printk("Could not get MMA8x5x device\n");
        return;
    }

    while (1) {
        struct sensor_value accel_value;

        // Fetch the sensor value
        sensor_sample_fetch(mma8x5x_dev);
        sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, &accel_value);

        // Print out the sensor value
        printk("Accel X: %d, Y: %d, Z: %d\n", accel_value.val1, accel_value.val2, accel_value.val3);

        // Store the sensor reading to NVS
        rc = nvs_write(&nvs, 1, &accel_value, sizeof(accel_value));
        if (rc < 0) {
            printk("Failed to store data in NVS\n");
        }

        k_msleep(SLEEP_TIME_MS);
    }
}