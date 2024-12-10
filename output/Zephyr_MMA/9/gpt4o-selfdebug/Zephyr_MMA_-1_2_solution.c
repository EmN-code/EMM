#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#define DELAY_SECONDS 5

static void read_accelerometer_and_store(void) {
    const struct device *dev = device_get_binding("MMA8x5x");
    if (!dev) {
        printk("Could not find MMA8x5x device!\n");
        return;
    }

    struct sensor_value accel[3];

    while (1) {
        sensor_sample_fetch(dev);
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        printk("Accel (m/s^2): X=%d, Y=%d, Z=%d\n", accel[0].val1, accel[1].val1, accel[2].val1);

        // Here we would store the values using a suitable method other than the unavailable NVS

        k_sleep(K_SECONDS(DELAY_SECONDS));
    }
}

int main(void) {
    printk("Starting application\n");
    read_accelerometer_and_store();
    return 0;
}