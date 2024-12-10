#include <zephyr/storage/flash_map.h>
#include <zephyr/settings/settings.h>
#include <zephyr/kernel.h>
#include <device.h>
#include <drivers/sensor.h>

#define SLEEP_TIME_MS   (5000)

/* Define the NVS file system */
static const struct flash_area *fa;

void main(void) {
    const struct device *sensor_dev;
    int rc;

    /* Initialize the NVS subsystem */
    rc = flash_area_open(DT_FLASH_AREA_STORAGE_ID, &fa);
    if (rc < 0) {
        printk("Failed to open NVS storage area\n");
        return;
    }

    /* Attempt to mount the filesystem */
    struct fs_mount_t nvs_mnt = {
        .mnt_point = "/nvs",
    };
    rc = fs_mount(&nvs_mnt);
    if (rc < 0) {
        printk("Failed to mount NVS\n");
        return;
    }

    /* Setup MMA8x5x sensor */
    sensor_dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8652fc)));
    if (!sensor_dev) {
        printk("Failed to get sensor binding\n");
        return;
    }

    struct sensor_value accel[3];

    printk("ESP32 MMA8x5x Reading NVS Example\n");

    while (1) {
        /* Read sensor data */
        rc = sensor_sample_fetch_chan(sensor_dev, SENSOR_CHAN_ACCEL_XYZ);
        if (rc < 0) {
            printk("Failed to fetch data from sensor\n");
        }

        rc = sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc < 0) {
            printk("Failed to get data from sensor\n");
        } else {
            printk("Acceleration X: %d.%06d, Y: %d.%06d, Z: %d.%06d\n",
                   accel[0].val1, accel[0].val2,
                   accel[1].val1, accel[1].val2,
                   accel[2].val1, accel[2].val2);

            /* Convert data to save in NVS */
            char buf[64];
            snprintf(buf, sizeof(buf), "%d.%06d,%d.%06d,%d.%06d",
                     accel[0].val1, accel[0].val2,
                     accel[1].val1, accel[1].val2,
                     accel[2].val1, accel[2].val2);

            rc = settings_save_one("/nvs/last_accel", buf, strlen(buf) + 1);
            if (rc < 0) {
                printk("Failed to save acceleration data to NVS\n");
            }
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    /* Finalize and close flash area */
    flash_area_close(fa);
}