#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/flash_map_defs.h>
#include <zephyr/storage/nvs/nvs.h>

#define SLEEP_TIME_MS   5000
#define SENSOR_NAME     "MMA8x5x"
#define NVS_STORAGE_ID  1

static struct nvs_fs fs;
static struct flash_area const *fa;

void main(void) {
    int rc;
    struct sensor_value accel[3];
    const struct device *sensor = device_get_binding(SENSOR_NAME);

    if (!sensor) {
        printk("No device found\n");
        return;
    }

    fs.storage_id = NVS_STORAGE_ID;
    rc = flash_area_open(NVS_STORAGE_ID, &fa);
    if (rc < 0) {
        printk("Flash area open error %d\n", rc);
        return;
    }

    fs.flash_area = fa;
    rc = nvs_init(&fs, sensor);
    if (rc < 0) {
        printk("NVS initialization error %d\n", rc);
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(sensor);
        if (rc < 0) {
            printk("Sensor sample update error\n");
            return;
        }

        rc = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc < 0) {
            printk("Cannot read sensor channels\n");
            return;
        }
        
        rc = nvs_write(&fs, 1, &accel, sizeof(accel));
        if (rc < 0) {
            printk("NVS write error %d\n", rc);
        } else {
            printk("Acceleration X:%d, Y:%d, Z:%d\n", accel[0].val1, accel[1].val1, accel[2].val1);
        }

        k_msleep(SLEEP_TIME_MS);
    }
}