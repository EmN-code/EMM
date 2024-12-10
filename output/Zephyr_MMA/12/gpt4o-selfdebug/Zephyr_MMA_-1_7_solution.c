#include <zephyr/types.h>
#include <zephyr/tracing/tracing_macros.h>
#include <device.h>
#include <drivers/sensor.h>
#include <nvs/nvs.h>
#include <zephyr.h>

#define STORAGE_FLASH_PAGE_SIZE 4096
#define STORAGE_FLASH_PAGE_COUNT 32
#define SENSOR_READ_INTERVAL K_SECONDS(5)

static struct nvs_fs fs;

void main(void) {
    const struct device *sensor_dev;
    struct sensor_value accel[3];
    int rc;

    sensor_dev = device_get_binding("MMA8X5X");
    if (!sensor_dev) {
        printk("No device found.\n");
        return;
    }

    fs.offset = FLASH_AREA_OFFSET(storage);
    fs.sector_size = STORAGE_FLASH_PAGE_SIZE;
    fs.sector_count = STORAGE_FLASH_PAGE_COUNT;

    rc = nvs_init(&fs, DT_FLASH_SIZE);
    if (rc) {
        printk("NVS Init error: %d\n", rc);
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(sensor_dev);
        if (rc) {
            printk("Sensor fetch error\n");
            continue;
        }

        sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        printk("Accel X: %d, Y: %d, Z: %d\n", accel[0].val1, accel[1].val1, accel[2].val1);

        int32_t accel_value = accel[0].val1;
        nvs_write(&fs, 1, &accel_value, sizeof(accel_value));

        k_sleep(SENSOR_READ_INTERVAL);
    }
}