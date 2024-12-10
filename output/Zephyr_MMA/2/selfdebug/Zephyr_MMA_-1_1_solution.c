#include <zephyr.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <storage/stream_flash.h>

#define SENSOR_NODE DT_ALIAS(mma8x5x)

#if !DT_NODE_HAS_STATUS(SENSOR_NODE, okay)
#error "Unsupported board: mma8x5x sensor not found"
#endif

#define STORAGE_PARTITION_LABEL storage

const struct flash_area *storage_area;
struct stream_flash_ctx storage_ctx;

void save_to_nvs(float reading) {
    int err = flash_area_open(flash_area_id_from_label(STORAGE_PARTITION_LABEL), &storage_area);
    if (err) {
        printk("Error opening flash area: %d\n", err);
        return;
    }

    printk("Saving reading to NVS: %f\n", reading);

    stream_flash_init(&storage_ctx, storage_area, NULL, 0, 0, 0, NULL);
    stream_flash_buffered_write(&storage_ctx, (const uint8_t*)&reading, sizeof(reading), true);

    flash_area_close(storage_area);
}

void read_sensor_and_store(const struct device *sensor) {
    struct sensor_value value;
    int ret = sensor_sample_fetch(sensor);
    if (ret) {
        printk("Error fetching sensor sample: %d\n", ret);
        return;
    }

    ret = sensor_channel_get(sensor, SENSOR_CHAN_ALL, &value);
    if (ret) {
        printk("Error retrieving sensor data: %d\n", ret);
        return;
    }

    float reading = sensor_value_to_double(&value);
    printk("Read value: %f\n", reading);

    save_to_nvs(reading);
}

void main(void) {
    const struct device *dev = DEVICE_DT_GET(SENSOR_NODE);

    if (!device_is_ready(dev)) {
        printk("Sensor device is not ready\n");
        return;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }
}