#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/fs/nvs.h>

#define SENSOR_LABEL "MMA8x5x"
#define SLEEP_TIME_MS 5000

static struct nvs_fs fs;

void main(void) {
    const struct device *sensor = device_get_binding(SENSOR_LABEL);

    if (!device_is_ready(sensor)) {
        printk("Sensor %s not ready\n", SENSOR_LABEL);
        return;
    }

    struct sensor_value accel[3];
    int rc;

    /* Initialize the NVS file system */
    struct flash_pages_info info;
    fs.sector_size = 4096;
    fs.sector_count = 3;
    fs.offset = FLASH_AREA_OFFSET(storage);
    nvs_mount(&fs);

    while (1) {
        rc = sensor_sample_fetch(sensor);
        if (rc < 0) {
            printk("Failed to fetch data from sensor: %d\n", rc);
            continue;
        }

        rc = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc < 0) {
            printk("Failed to get data from sensor: %d\n", rc);
            continue;
        }

        printk("Acceleration X: %d, Y: %d, Z: %d\n",
               accel[0].val1, accel[1].val1, accel[2].val1);

        nvs_write(&fs, 1, accel, sizeof(accel));

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}