#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/settings/settings.h>
#include <zephyr/storage/flash_map.h>

#define I2C_NODE DT_NODELABEL(i2c0)
#define MMA8X5X_NODE DT_NODELABEL(mma845x)
#define SENSOR_INTERVAL K_SECONDS(5)
#define NVS_ID 1

static const struct device *i2c_dev = DEVICE_DT_GET(I2C_NODE);
static const struct device *mma8x5x_dev = DEVICE_DT_GET(MMA8X5X_NODE);

void sensor_read_thread(void)
{
    const struct device *dev = mma8x5x_dev;
    struct sensor_value accel[3];

    while (1) {
        if (sensor_sample_fetch(dev) < 0) {
            printk("Failed to fetch sensor data\n");
            continue;
        }

        if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
            printk("Failed to get sensor channel\n");
            continue;
        }

        printk("Accel X: %d, Y: %d, Z: %d\n",
               accel[0].val1, accel[1].val1, accel[2].val1);

        // Store the first axis value in NVS
        settings_runtime_set("/nvs/accel_x", &accel[0].val1, sizeof(accel[0].val1));

        k_sleep(SENSOR_INTERVAL);
    }
}

void main(void)
{
    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not found\n");
        return;
    }

    if (!device_is_ready(mma8x5x_dev)) {
        printk("MMA8X5X device not found\n");
        return;
    }

    settings_runtime_load();

    k_thread_create(K_THREAD_STACK_DEFINE(sensor_stack, 1024),
                    sensor_stack, K_THREAD_STACK_SIZEOF(sensor_stack),
                    (k_thread_entry_t)sensor_read_thread,
                    NULL, NULL, NULL,
                    K_PRIO_PREEMPT(1), 0, K_NO_WAIT);

    printk("MMA8X5X sensor monitoring started\n");
}