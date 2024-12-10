// Required header files for Zephyr and ESP32
#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
// Include NVS and fs headers for persistent storage
#include <zephyr/fs/nvs.h>

#define I2C_NODE DT_INST(0, nxp_mma8x5x)

// Sleep time for the periodic reading
#define SLEEP_TIME_MS 5000

void main(void) {
    const struct device *mma8x5x_dev;
    struct sensor_value accel[3];
    int rc;
    
    // Initialize NVS file system
    static struct flash_pages_layout layout;
    static struct nvs_fs fs;

    fs.offset = DT_CHOSEN(zephyr_flash);
    fs.sector_size = layout.pages_size;
    fs.sector_count = 3U;

    rc = nvs_init(&fs, DT_CHOSEN(zephyr_flash_controller));
    if (rc) {
        printk("NVS initialization failed\n");
        return;
    }

    mma8x5x_dev = device_get_binding(DT_LABEL(I2C_NODE));
    if (!mma8x5x_dev) {
        printk("No device found\n");
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(mma8x5x_dev);
        if (rc == 0) {
            rc = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
            printk("X: %.2f, Y: %.2f, Z: %.2f\n",
                   sensor_value_to_double(&accel[0]),
                   sensor_value_to_double(&accel[1]),
                   sensor_value_to_double(&accel[2]));

            // Store the readings in NVS
            rc = nvs_write(&fs, 1, &accel, sizeof(accel));
            if (rc < 0) {
                printk("Failed to write to NVS\n");
            }
        } else {
            printk("Failed to fetch sample\n");
        }
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}