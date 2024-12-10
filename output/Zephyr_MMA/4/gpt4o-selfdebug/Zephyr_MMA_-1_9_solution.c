#include <zephyr/zephyr.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>

#define SLEEP_TIME_MS   5000
#define STORAGE_PARTITION     storage

void main(void) {
    const struct device *sensor = device_get_binding("MMA8x5x");
    if (!sensor) {
        printk("Could not get MMA8x5x device binding\n");
        return;
    }

    while (1) {
        struct sensor_value accel[3];
        if (sensor_sample_fetch(sensor) < 0) {
            printk("Sensor sample update error\n");
            continue;
        }
        if (sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
            printk("Cannot read MMA8x5x channels\n");
            continue;
        }

        printk("ax: %d.%06d; ay: %d.%06d; az: %d.%06d\n", 
            accel[0].val1, accel[0].val2, 
            accel[1].val1, accel[1].val2, 
            accel[2].val1, accel[2].val2);

        // Simulate storing in NVS
        printk("Simulating storing values in NVS...\n");

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}