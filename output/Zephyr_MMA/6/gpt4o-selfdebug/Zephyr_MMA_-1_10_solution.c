#include <zephyr.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/nvs/nvs.h>

#define MMA8x5x_NODE DT_ALIAS(mma8451)
#define STORAGE_PARTITION FLASH_AREA_STORAGE_OFFSET
#define STORAGE_SIZE 4096

void main(void) {
    const struct device *sensor = DEVICE_DT_GET(MMA8x5x_NODE);
    struct sensor_value accel_value;
    struct nvs_fs fs;
    int rc;

    if (!device_is_ready(sensor)) {
        printk("Sensor device not ready.\n");
        return;
    }

    fs.offset = STORAGE_PARTITION;
    fs.sector_size = STORAGE_SIZE;
    nvs_init(&fs, "storage");

    while (1) {
        rc = sensor_sample_fetch(sensor);
        if (rc == 0) {
            rc = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, &accel_value);
            if (rc == 0) {
                printk("Acceleration read: %d.%06d\n", accel_value.val1, accel_value.val2);
                rc = nvs_write(&fs, 1, &accel_value, sizeof(accel_value));
                if (rc > 0) {
                    printk("Written acceleration to NVS.\n");
                }
            }
        }
        k_sleep(K_SECONDS(5));
    }
}