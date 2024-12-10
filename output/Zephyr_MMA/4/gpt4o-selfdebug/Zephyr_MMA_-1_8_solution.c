#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>

#define SLEEP_TIME_MS   5000
#define SENSOR_LABEL    DT_LABEL(DT_INST(0, nxp_mma8652)) // Replace with actual sensor label
#define NVS_PARTITION   fixed-partition-name // Replace with actual partition name

static struct nvs_fs fs;

void main(void) {
    const struct device *sensor_dev;
    struct sensor_value accel[3];
    int rc;

    /* Setup flash partition for NVS */
    struct flash_area *fa;
    rc = flash_area_open(FLASH_AREA_ID(NVS_PARTITION), &fa);
    if (rc) {
        printk("Flash area open failed: %d\n", rc);
        return;
    }

    fs.offset = FLASH_AREA_OFFSET(NVS_PARTITION);
    fs.sector_size = 4096U;
    fs.sector_count = 3U;
    nvs_mount(&fs);

    /* Initialize sensor device */
    sensor_dev = device_get_binding(SENSOR_LABEL);
    if (!sensor_dev) {
        printk("Failed to get sensor binding.\n");
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(sensor_dev);
        if (rc) {
            printk("Sensor sample fetch failed: %d\n", rc);
        } else {
            sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
            printk("x: %d, y: %d, z: %d\n", accel[0].val1, accel[1].val1, accel[2].val1);

            /* Save the reading in NVS */
            nvs_write(&fs, RECORD_ACCEL, accel, sizeof(accel));
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}