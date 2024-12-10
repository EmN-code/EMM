#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/settings/nvs.h>

#define SENSOR_NODE DT_ALIAS(mma8x5x)
#define NVS_STORAGE_PARTITION "storage"

void main(void) {
    const struct device *sensor = DEVICE_DT_GET(SENSOR_NODE);
    struct sensor_value accel_value;
    struct nvs_fs fs; 
    int rc;

    if (!device_is_ready(sensor)) {
        printk("Sensor not found!");
        return;
    }

    struct flash_pages_info info;
    rc = flash_get_page_info_by_offs(device_get_binding(NVS_STORAGE_PARTITION), 0, &info);
    
    if (rc) {
        printk("Failed to get flash page info");
        return;
    }

    fs.offset = info.start_offset;
    fs.sector_size = info.size;
    rc = nvs_mount(&fs);

    if (rc) {
        printk("NVS mount failed");
        return;
    }

    k_msleep(5000);

    while (1) {
        if (sensor_sample_fetch(sensor) < 0) {
            printk("Sensor fetch failed");
            continue;
        }

        sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, &accel_value);
        printk("Acceleration X: %d", accel_value.val1);

        /* Store the reading in NVS */
        rc = nvs_write(&fs, 1, &accel_value.val1, sizeof(accel_value.val1));
        if (rc < 0) {
            printk("Failed to write to NVS");
        }

        k_msleep(5000); // Sleep for 5 seconds
    }
}