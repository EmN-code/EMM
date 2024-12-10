#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/nvs.h>

#define SLEEP_TIME_MS   5000

void main(void) {
    struct nvs_fs nvs;
    struct sensor_value accel_value;
    const struct device *mma8x5x_dev;
    int ret;

    /* Initialize the NVS file system */
    nvs.offset = FLASH_AREA_OFFSET(storage);
    nvs.sector_size = FLASH_AREA_SIZE(storage);
    nvs.sector_count = 3;

    /* Mount the NVS file system */
    ret = nvs_mount(&nvs);
    if (ret != 0) {
        printk("Failed to mount NVS (%d)\n", ret);
        return;
    }

    /* Get the device binding for the accelerometer */
    mma8x5x_dev = device_get_binding("MMA8x5x");
    if (!mma8x5x_dev) {
        printk("No device found\n");
        return;
    }

    while (1) {
        /* Sample the accelerometer */
        ret = sensor_sample_fetch(mma8x5x_dev);
        if (ret) {
            printk("Sensor sample fetch error (%d)\n", ret);
            continue;
        }

        /* Read the accelerometer value */
        ret = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, &accel_value);
        if (ret) {
            printk("Sensor channel get error (%d)\n", ret);
            continue;
        }

        /* Store the accelerometer value in NVS */
        ret = nvs_write(&nvs, 1, &accel_value, sizeof(accel_value));
        if (ret < 0) {
            printk("NVS write error (%d)\n", ret);
        }

        printk("Stored accelerometer value (x,y,z): %d.%06d, %d.%06d, %d.%06d\n",
               accel_value.val1, accel_value.val2,
               accel_value.val1, accel_value.val2,
               accel_value.val1, accel_value.val2);

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}