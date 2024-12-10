#include <zephyr/types.h>
#include <zephyr/nvs/nvs.h>
#include <zephyr/kernel.h>
#include <device.h>
#include <drivers/sensor/mma8x5x.h>

#define SLEEP_TIME_MS   5000
#define NVS_ID          1

void main(void) {
    const struct device *sensor;
    struct sensor_value accel[3];
    struct nvs_fs fs;
    int rc;

    /* Define file system configurations */
    fs.offset = FLASH_AREA_OFFSET(storage);
    fs.sector_size = FLASH_AREA_SIZE(storage);
    fs.sector_count = 3;
    rc = nvs_init(&fs, DT_LABEL(DT_CHOSEN(zephyr_flash_controller)));
    if (rc) {
        printk("NVS initialization failed\n");
        return;
    }

    /* Get sensor device binding */
    sensor = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8x5x)));
    if (!sensor) {
        printk("No sensor device found\n");
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(sensor);
        if (rc) {
            printk("Failed to fetch sensor sample\n");
            continue;
        }
        
        rc = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc) {
            printk("Failed to get sensor channel\n");
        } else {
            printk("Accel X: %d, Y: %d, Z: %d\n", accel[0].val1, accel[1].val1, accel[2].val1);
            /* Store the last X value */
            rc = nvs_write(&fs, NVS_ID, &accel[0], sizeof(accel[0]));
            if (rc < 0) {
                printk("Failed to write to NVS\n");
            } else {
                printk("Stored accel X: %d\n", accel[0].val1);
            }
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}