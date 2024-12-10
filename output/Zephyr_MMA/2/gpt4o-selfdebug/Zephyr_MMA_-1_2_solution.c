#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/flash_area.h>
#include <zephyr/storage/nvs.h>

#define SLEEP_TIME_MS   (5000)
#define SENSOR_NODE DT_PATH(soc, sensor)

void mma8x5x_read_and_store(struct k_timer *dummy);

struct nvs_fs fs;
struct sensor_value accel[3];

void main(void) {
    // Initialize the NVS file system
    fs.storage = (void *)FLASH_AREA_ID(storage);
    if (nvs_mount(&fs) < 0) {
        printk("Failed to mount the NVS file system\n");
        return;
    }

    const struct device *sensor = device_get_binding(DT_LABEL(SENSOR_NODE));
    if (!sensor) {
        printk("Sensor device not found\n");
        return;
    }

    while (1) {
        // Fetch sensor data
        if (sensor_sample_fetch(sensor) < 0) {
            printk("Failed to fetch sample\n");
            k_sleep(K_MSEC(SLEEP_TIME_MS));
            continue;
        }

        // Get accelerometer data
        sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel);

        // Store data using NVS
        struct nvs_ate data_ate;

        // Writing accelerometer X axis
        if (nvs_write(&fs, NVS_KEY_START, &accel[0], sizeof(accel[0])) < 0) {
            printk("Writing NVS failed!\n");
        }

        printk("Accel X: %d, Y: %d, Z: %d\n", accel[0].val1, accel[1].val1, accel[2].val1);
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}