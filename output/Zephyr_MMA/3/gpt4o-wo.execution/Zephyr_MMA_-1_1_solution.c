#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/drivers/nvs.h>
#include <zephyr/drivers/i2c.h>

#define SLEEP_TIME_MS 5000
#define NVS_ID 1

void main(void) {
    const struct device *sensor_dev;
    const struct device *nvs_dev;
    struct sensor_value accel_value;
    int err;

    sensor_dev = device_get_binding("MMA8451");
    if (!sensor_dev) {
        printk("Failed to find sensor device\n");
        return;
    }

    nvs_dev = device_get_binding("NVS_0");
    if (!nvs_dev) {
        printk("Failed to initialize NVS\n");
        return;
    }

    while (1) {
        err = sensor_sample_fetch(sensor_dev);
        if (err) {
            printk("Failed to fetch sensor sample (%d)\n", err);
            continue;
        }

        err = sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, &accel_value);
        if (err) {
            printk("Failed to get sensor channel (%d)\n", err);
            continue;
        }

        err = nvs_write(nvs_dev, NVS_ID, &accel_value, sizeof(accel_value));
        if (err < 0) {
            printk("Failed to write to NVS (%d)\n", err);
        }

        k_msleep(SLEEP_TIME_MS);
    }
}