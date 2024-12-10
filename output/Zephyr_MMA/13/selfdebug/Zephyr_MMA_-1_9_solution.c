
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/settings/settings.h>
#include <stdio.h>

#define MMA8x5x_DEV_NAME DT_LABEL(DT_INST(0, nxp_mma8x5x))

static struct sensor_value accel_value;

void save_accel_value(void)
{
    int ret;
    char buf[32];

    snprintf(buf, sizeof(buf), "%d.%06d", accel_value.val1, accel_value.val2);

    ret = settings_save_one("sensor/accel", buf, strlen(buf) + 1);
    if (ret) {
        printk("Failed to save accel value: %d\n", ret);
    } else {
        printk("Accel value saved: %s\n", buf);
    }
}

void read_accel_value(void)
{
    const struct device *dev = device_get_binding(MMA8x5x_DEV_NAME);
    if (!dev) {
        printk("Device %s not found\n", MMA8x5x_DEV_NAME);
        return;
    }

    if (sensor_sample_fetch(dev)) {
        printk("Failed to fetch sample\n");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &accel_value)) {
        printk("Failed to get sample\n");
        return;
    }

    printk("Accel value: %d.%06d\n", accel_value.val1, accel_value.val2);
}

void main(void)
{
    int ret;

    ret = settings_subsys_init();
    if (ret) {
        printk("Settings subsys init failed: %d\n", ret);
        return;
    }

    while (1) {
        read_accel_value();
        save_accel_value();
        k_sleep(K_SECONDS(5));
    }
}
