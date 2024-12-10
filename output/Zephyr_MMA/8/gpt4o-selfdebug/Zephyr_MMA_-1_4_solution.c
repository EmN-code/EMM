#include <zephyr/kernel.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#define SLEEP_TIME_MS 5000
#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 3

static struct nvs_fs nvs;
static struct flash_pages_info info;

void main(void) {
    int rc;
    const struct device *sensor_dev;
    struct sensor_value accel_x;

    // Initialize NVS
    nvs.sector_size = NVS_SECTOR_SIZE;
    nvs.sector_count = NVS_SECTOR_COUNT;
    nvs.offset = DT_FLASH_AREA_STORAGE_OFFSET;

    rc = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL),
                                     nvs.offset, &info);
    if (rc) {
        printk("Failed to get page info!
");
        return;
    }

    rc = nvs_mount(&nvs);
    if (rc) {
        printk("NVS mount error: %d
", rc);
        return;
    }

    // Initialize the sensor device
    sensor_dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8652fc)));
    if (!sensor_dev) {
        printk("Failed to find sensor device!
");
        return;
    }

    while (1) {
        // Fetch sensor data
        rc = sensor_sample_fetch(sensor_dev);
        if (rc) {
            printk("Sensor sample fetch error: %d
", rc);
            continue;
        }

        rc = sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_X, &accel_x);
        if (rc) {
            printk("Cannot read sensor channel: %d
", rc);
            continue;
        }

        printk("Accel X: %d.%06d
", accel_x.val1, accel_x.val2);
        
        // Store the value in NVS
        rc = nvs_write(&nvs, 1, &accel_x, sizeof(accel_x));
        if (rc < 0) {
            printk("NVS write error: %d
", rc);
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}