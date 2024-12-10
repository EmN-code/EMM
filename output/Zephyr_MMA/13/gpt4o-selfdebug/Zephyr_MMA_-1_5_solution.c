#include <zephyr/types.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/flash.h>
#include <zephyr/storage/nvs.h>
#include <zephyr/kernel.h> 

#define SLEEP_TIME_MS   5000
#define STORAGE_ID      1
#define SENSOR_NODE DT_NODELABEL(mma_sensor)

struct nvs_fs fs;

static int init_flash_nvs(void) {
    int err;
    struct flash_pages_info info;

    fs.offset = FLASH_AREA_OFFSET(storage);

    err = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER), fs.offset, &info);
    if (err) {
        printk("Flash page info failed: error %d\n", err);
        return err;
    }

    fs.sector_size = info.size;
    fs.sector_count = 3;
    fs.flash_dev = device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER);

    err = nvs_init(&fs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER);
    if (err) {
        printk("NVS Initialization failed: %d\n", err);
        return err;
    }
    return 0;
}

void main(void) {
    const struct device *sensor;
    int ret;
    struct sensor_value accel_value;

    ret = init_flash_nvs();
    if (ret) {
        printk("Flash/NVS initialization error\n");
        return;
    }

    sensor = device_get_binding(DT_LABEL(SENSOR_NODE));
    if (sensor == NULL) {
        printk("No device found\n");
        return;
    }

    while (1) {
        ret = sensor_sample_fetch(sensor);
        if (ret) {
            printk("Sensor sample update error %d\n", ret);
            continue;
        }

        ret = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, &accel_value);
        if (ret) {
            printk("Cannot read sensor channel\n");
            continue;
        }

        printk("Accel: %.2f\n", sensor_value_to_double(&accel_value));

        ret = nvs_write(&fs, STORAGE_ID, &accel_value, sizeof(accel_value));
        if (ret < 0) {
            printk("NVS write error: %d\n", ret);
        }

        k_msleep(SLEEP_TIME_MS);
    }
}