#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/nvs/nvs.h>
#include <zephyr/kernel.h>
#include <device.h>
#include <drivers/sensor.h>

#define SLEEP_TIME_MS   (5 * 1000)
#define SENSOR_NODE DT_NODELABEL(mma8x5x)

void main(void) {
    const struct device *sensor;
    struct sensor_value accel[3];
    struct nvs_fs fs;
    int rc;

    sensor = DEVICE_DT_GET(SENSOR_NODE);
    if (!device_is_ready(sensor)) {
        printk("Sensor device not ready\n");
        return;
    }

    fs.offset = CONFIG_NVS_STORAGE_OFFSET;
    fs.sector_count = 3;
    fs.sector_size = 4096;

    rc = nvs_init(&fs, NULL);
    if (rc) {
        printk("Failed to initialize NVS\n");
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(sensor);
        if (rc) {
            printk("Sensor fetch failed\n");
            return;
        }

        rc = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc) {
            printk("Failed to get sensor data\n");
            return;
        }

        printk("Accel X: %d, Y: %d, Z: %d\n", accel[0].val1, accel[1].val1, accel[2].val1);

        rc = nvs_write(&fs, 1, &accel, sizeof(accel));
        if (rc < 0) {
            printk("Failed to write to NVS\n");
            return;
        }

        k_msleep(SLEEP_TIME_MS);
    }
}