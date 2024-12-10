#include <zephyr/storage/flash_map.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <drivers/sensor.h>

#define MMA8X5X_DEV_LABEL "MMA8X5X"
#define READ_INTERVAL 5000 /* 5 seconds in milliseconds */

void main(void) {
    const struct device *mma8x5x_dev = device_get_binding(MMA8X5X_DEV_LABEL);
    if (!mma8x5x_dev) {
        printk("Device %s not found\n", MMA8X5X_DEV_LABEL);
        return;
    }

    struct sensor_value accel_x, accel_y, accel_z;
    
    while (1) {
        if (sensor_sample_fetch(mma8x5x_dev) < 0) {
            printk("Failed to fetch sensor data\n");
            k_sleep(K_MSEC(READ_INTERVAL));
            continue;
        }
        
        sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_X, &accel_x);
        sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_Y, &accel_y);
        sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_Z, &accel_z);

        /* Print the sensor data */
        printk("Acceleration X: %d, Y: %d, Z: %d\n", accel_x.val1, accel_y.val1, accel_z.val1);

        /* You can add NVS storage write operation here */

        k_sleep(K_MSEC(READ_INTERVAL));
    }
}