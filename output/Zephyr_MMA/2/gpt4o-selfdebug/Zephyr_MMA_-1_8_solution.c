#include <zephyr.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/settings/nvs.h>

#define SENSOR_NODE DT_INST(0, nxp_mma8652fc)

/* NVS storage definitions */
static struct nvs_fs fs;
static uint8_t sensor_value;

void store_value(uint8_t value) {
    int err; 
    err = nvs_write(&fs, 1, &value, sizeof(value));
    if (err < 0) {
        printk("Error writing to NVS: %d\n", err);
    }
}

void main(void) {
    const struct device *dev = DEVICE_DT_GET(SENSOR_NODE);
    int rc;

    /* NVS setup */
    fs.offset = DT_FLASH_AREA_STORAGE_OFFSET;
    rc = nvs_init(&fs, DT_FLASH_AREA_STORAGE_PAGE_SIZE);
    if (rc) {
        printk("NVS error: %d\n", rc);
        return;
    }

    if (!device_is_ready(dev)) {
        printk("Sensor device not found\n");
        return;
    }

    struct sensor_value accel_value;

    while (true) {
        rc = sensor_sample_fetch(dev);
        if (rc) {
            printk("Failed to fetch sensor sample: %d\n", rc);
            continue;
        }

        rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &accel_value);
        if (rc) {
            printk("Failed to get sensor data: %d\n", rc);
            continue;
        }

        sensor_value = accel_value.val1;
        store_value(sensor_value);
        printk("Stored accelerometer value: %d\n", sensor_value);

        k_sleep(K_SECONDS(5));
    }
}