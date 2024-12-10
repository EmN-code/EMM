#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>

#define STORAGE_FLASH_AREA "storage"

void main(void) {
    struct device *mma8x5x_dev;
    struct sensor_value accel_value;
    struct flash_area *fa;
    struct nvs_fs nvs;

    // Bind the MMA8x5x sensor
    mma8x5x_dev = device_get_binding("MMA8x5x");
    if (!mma8x5x_dev) {
        printk("Device not found\n");
        return;
    }

    // NVS configuration
    nvs.sector_size = FLASH_AREA_SIZE(storage);
    flash_area_open(FLASH_AREA_ID(storage), &fa);
    nvs.offset = fa->fa_off;
    nvs.device = flash_area_get_device(fa);

    nvs_init(&nvs, "MMA8x5x");

    while (1) {
        // Fetch sensor data
        if (sensor_sample_fetch(mma8x5x_dev) < 0) {
            printk("Sensor sample update error\n");
            return;
        }

        // Get sensor value
        if (sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, &accel_value) < 0) {
            printk("Cannot read sensor channels\n");
        } else {
            printk("Acceleration: x: %d.%06d\n", accel_value.val1, accel_value.val2);
            // Store the value using NVS
            nvs_write(&nvs, 1, &accel_value, sizeof(accel_value));
        }

        // Wait for 5 seconds
        k_sleep(K_SECONDS(5));
    }
}