#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/nvs/nvs.h>
#include <zephyr/device.h>

#define STORAGE_PARTITION_LABEL storage
#define MMA8X5X_LABEL "MMA8x5x"

static struct nvs_fs fs;
static const struct device *sensor_dev;

void configure_nvs(void) {
    struct flash_pages_info info;
    fs.offset = DT_FLASH_AREA_STORAGE_OFFSET;

    int rc = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL), fs.offset, &info);
    if (rc) {
        printk("Failed to get flash page info: %d\n", rc);
        return;
    }
    fs.sector_size = info.size;
    fs.sector_count = 3U;

    rc = nvs_init(&fs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
    if (rc) {
        printk("NVS Init failed: %d\n", rc);
    }
}

void read_and_store_sensor_data(void) {
    struct sensor_value accel[3];
    int rc = sensor_sample_fetch(sensor_dev);

    if (rc) {
        printk("Sensor sample update error: %d\n", rc);
        return;
    }

    rc = sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (rc) {
        printk("Cannot read sensor channels: %d\n", rc);
        return;
    }

    printk("x: %d\ty: %d\tz: %d\n",
           accel[0].val1, accel[1].val1, accel[2].val1);

    /* Store in NVS */
    rc = nvs_write(&fs, 1, &accel, sizeof(accel));
    if (rc < 0) {
        printk("Failed to store data in NVS: %d\n", rc);
    } else {
        printk("Stored data in NVS\n");
    }
}

void main(void) {
    printk("Starting MMA8x5x sensor reading example\n");

    configure_nvs();

    sensor_dev = device_get_binding(MMA8X5X_LABEL);
    if (!sensor_dev) {
        printk("No device found for %s\n", MMA8X5X_LABEL);
        return;
    }

    while (1) {
        read_and_store_sensor_data();
        k_sleep(K_SECONDS(5));
    }
}