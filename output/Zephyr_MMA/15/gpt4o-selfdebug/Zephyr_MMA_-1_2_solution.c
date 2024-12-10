#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/syscalls/i2c.h> 
#include <zephyr/kernel_includes.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/init.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x) // Placeholder, replace with actual node label
#define I2C_NODE DT_NODELABEL(i2c0)       // Placeholder, replace with actual node label

void main(void) {
    const struct device *mma8x5x_dev = device_get_binding(DT_LABEL(MMA8X5X_NODE));

    if (!mma8x5x_dev) {
        printk("Failed to find sensor device\n");
        return;
    }

    struct sensor_value accel;
    struct nvs_fs fs = {
        .sector_size = FLASH_SECTOR_SIZE,
        .sector_count = 3,
        .offset = DT_FLASH_AREA_STORAGE_OFFSET,
    };

    nvs_mount(&fs);

    while (1) {
        if (sensor_sample_fetch(mma8x5x_dev) < 0) {
            printk("Sensor fetch error\n");
        }

        if (sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, &accel) < 0) {
            printk("Sensor channel fetch error\n");
        } else {
            nvs_write(&fs, 1, &accel, sizeof(accel));
            printk("Accel Value: %d\n", accel.val1);
        }

        k_sleep(K_SECONDS(5));
    }
}
