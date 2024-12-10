#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/settings/settings.h>

#define STACK_SIZE 1024
#define PRIORITY 7

K_THREAD_STACK_DEFINE(sensor_stack, STACK_SIZE);
struct k_thread sensor_thread;

void sensor_read_thread(void *p1, void *p2, void *p3) {
    const struct device *mma8x5x;
    struct sensor_value accel_values[3];

    mma8x5x = device_get_binding("MMA8x5x");
    if (!mma8x5x) {
        printk("No device found\n");
        return;
    }

    while (1) {
        sensor_sample_fetch(mma8x5x);
        sensor_channel_get(mma8x5x, SENSOR_CHAN_ACCEL_XYZ, accel_values);

        printk("Accel X: %d.%06d\n", accel_values[0].val1, accel_values[0].val2);
        printk("Accel Y: %d.%06d\n", accel_values[1].val1, accel_values[1].val2);
        printk("Accel Z: %d.%06d\n", accel_values[2].val1, accel_values[2].val2);

        // Store values in NVS (Non-Volatile Storage)
        settings_save_one("accel/x", &accel_values[0].val1, sizeof(accel_values[0].val1));
        settings_save_one("accel/y", &accel_values[1].val1, sizeof(accel_values[1].val1));
        settings_save_one("accel/z", &accel_values[2].val1, sizeof(accel_values[2].val1));

        k_sleep(K_SECONDS(5));
    }
}

void main(void) {
    k_thread_create(&sensor_thread, sensor_stack, K_THREAD_STACK_SIZEOF(sensor_stack),
                    sensor_read_thread, NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);

    settings_subsys_init();  // Initialize the settings subsystem
}