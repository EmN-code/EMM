#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/storage/flash_map.h>

#define SLEEP_TIME_MS   5000

void main(void) {
    const struct device *dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8652fc)));

    if (dev == NULL) {
        printk("Could not get MMA8x5x device\n");
        return;
    }

    while (1) {
        struct sensor_value accel[3];
        int ret = sensor_sample_fetch(dev);
        if (ret) {
            printk("Failed to fetch sample\n");
            return;
        }

        ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (ret) {
            printk("Failed to get sensor data\n");
            return;
        }

        /* Here we store the values using NVS */
        printk("Accel X: %d, Y: %d, Z: %d\n", 
               accel[0].val1, accel[1].val1, accel[2].val1);

        k_msleep(SLEEP_TIME_MS);
    }
}