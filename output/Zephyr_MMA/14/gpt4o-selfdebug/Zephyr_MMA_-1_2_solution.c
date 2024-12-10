#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/sensor.h>
#include <drivers/gpio.h>
#include <storage/flash_map.h>
#include <nvs/nvs.h>

#define SLEEP_TIME_MS   5000
#define SENSOR_NODE DT_NODELABEL(mma8x5x)

void main(void) {
    const struct device *sensor;
    struct sensor_value val;
    struct nvs_fs fs;
    struct flash_pages_info info;
    int err;

    // Initialize the sensor device
    sensor = device_get_binding(DT_LABEL(SENSOR_NODE));
    if (!sensor) {
        printk("Failed to find sensor device\n");
        return;
    }

    // Setup NVS
    fs.offset = FLASH_AREA_STORAGE_OFFSET;
    err = flash_get_page_info_by_offs(device_get_binding(FLASH_AREA_STORAGE_DEVICE), fs.offset, &info);
    if (err) {
        printk("Failed to get page info\n");
        return;
    }
    fs.sector_size = info.size;
    fs.sector_count = FLASH_AREA_STORAGE_SIZE / fs.sector_size;
    err = nvs_init(&fs, DT_LABEL(FLASH_AREA_STORAGE_NODE));
    if (err) {
        printk("NVS init failed\n");
        return;
    }

    // Main loop
    while (1) {
        // Fetch sensor data
        if (sensor_sample_fetch(sensor) < 0) {
            printk("Failed to fetch sensor sample\n");
        } else {
            sensor_channel_get(sensor, SENSOR_CHAN_ALL, &val);

            printk("Sensor value: %d.%06d\n", val.val1, val.val2);

            // Write reading into NVS
            err = nvs_write(&fs, 1, &val, sizeof(val));
            if (err < 0) {
                printk("Failed to write to NVS\n");
            }
        }

        // Sleep for defined interval
        k_msleep(SLEEP_TIME_MS);
    }
}