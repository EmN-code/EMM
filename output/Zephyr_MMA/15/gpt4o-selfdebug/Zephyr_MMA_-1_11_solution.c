// Description: This Zephyr application initializes and reads data from the MMA8x5x accelerometer every 5 seconds. It uses the NVS (Non-Volatile Storage) to store the reading persistently. The code includes necessary headers in the correct order, initializes the sensor and NVS, and schedules periodic reading and storage tasks.

#include <zephyr.h> // Zephyr's core kernel API
#include <drivers/sensor.h> // Sensor driver APIs
#include <device.h> // Device driver model API
#include <storage/flash_map.h> // Flash map support
#include <zephyr/drivers/storage/nvs.h> // Non-volatile storage driver

#define SLEEP_TIME_MS   5000
static struct sensor_value accel_value;
static const struct device *sensor_dev;
static struct nvs_fs nvs;

/* Erwegian TMP sensor mma8x5x device name. */
#define SENSOR_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8652fc))

void read_accel_and_store(void) {
    if (sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, &accel_value) == 0) {
        printk("Accelerometer reading: x=%d, y=%d, z=%d\n",
               accel_value.val1, accel_value.val2, accel_value.val3);
        
        /* Store the accelerometer readings into NVS */
        int rc = nvs_write(&nvs, 1, &accel_value, sizeof(accel_value));
        if (rc < 0) {
            printk("Failed to store accelerator data: %d\n", rc);
        } else {
            printk("Accelerometer data stored successfully.\n");
        }
    } else {
       printk("Failed to fetch accelerometer data\n");
    }
}

void main(void) {
    int rc;
    /* Initialize sensor */
    sensor_dev = device_get_binding(SENSOR_DEV_NAME);
    if (!sensor_dev) {
        printk("Failed to get sensor device binding\n");
        return;
    }
    printk("Sensor device %s is ready\n", SENSOR_DEV_NAME);

    /* Initialize NVS */
    struct flash_area *fa;
    rc = flash_area_open(FLASH_AREA_ID(storage), &fa);
    if (rc < 0) {
        printk("Error opening flash area\n");
        return;
    }

    nvs.offset = fa->fa_off;
    nvs.sector_size = fa->fa_size;
    nvs.sector_count = 3U;

    rc = nvs_init(&nvs, DT_FLASH_AREA_STORAGE_ID);
    if (rc) {
        printk("Cannot initialize NVS\n");
        return;
    }

    printk("NVS initialized successfully\n");

    while (1) {
        read_accel_and_store();
        k_msleep(SLEEP_TIME_MS);
    }
}
