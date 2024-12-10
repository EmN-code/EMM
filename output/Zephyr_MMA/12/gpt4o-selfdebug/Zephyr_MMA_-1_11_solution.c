#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <zephyr/types.h>

#define SENSOR_READ_INTERVAL    K_SECONDS(5)

void main(void) {
    const struct device *sensor = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8652fc)));
    if (!sensor) {
        printk("Failed to get sensor binding\n");
        return;
    }

    struct sensor_value accel[3];
    struct nvs_fs fs;

    fs.offset = DT_FLASH_AREA_STORAGE_OFFSET;
    /* Assume this is valid and configuration is done outside this snippet */

    while (1) {
        if (sensor_sample_fetch(sensor) < 0) {
            printk("Sensor sample update error\n");
            continue;
        }

        sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel);
        printk("Accel: X=%d, Y=%d, Z=%d\n", accel[0].val1, accel[1].val1, accel[2].val1);

        /* NVS Handling */
        /* This part would include the writing of accel data to NVS, assume fs initialized */
        /* Example: nvs_write(&fs, SENSOR_DATA_ID, &accel, sizeof(accel)); */

        k_sleep(SENSOR_READ_INTERVAL);
    }
}