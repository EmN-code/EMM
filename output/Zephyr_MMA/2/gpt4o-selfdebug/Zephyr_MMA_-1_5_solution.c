#include <zephyr/types.h>
#include <zephyr/sys/time_units.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/settings/nvs.h>

#define SLEEP_TIME_MS   5000

static struct nvs_fs fs;
#define STORAGE_PARTITION_LABEL "storage"
#define STORAGE_PARTITION_ID FIXED_PARTITION_ID(storage)

struct sensor_value accel_x, accel_y, accel_z;

void init_nvs(void) {
    struct flash_pages_info info;
    int rc;

    fs.offset = DT_FLASH_AREA_OFFSET(STORAGE_PARTITION_ID);
    rc = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL), fs.offset, &info);
    if (rc) {
        printk("Cannot get page info\n");
        return;
    }
    fs.sector_size = info.size;
    fs.sector_count = 3U;
    rc = nvs_init(&fs, STORAGE_PARTITION_LABEL);
    if (rc) {
        printk("Flash Init failed\n");
    }
}

void save_to_nvs(struct sensor_value *value) {
    int rc;
    rc = nvs_write(&fs, 1, value, sizeof(*value));
    if (rc < 0) {
        printk("Failed to write data to NVS\n");
    } else {
        printk("Data written to NVS\n");
    }
}

void read_sensor_data(const struct device *sensor) {
    if (sensor_sample_fetch(sensor) < 0) {
        printk("Failed to fetch data from sensor\n");
        return;
    }

    sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_X, &accel_x);
    sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_Y, &accel_y);
    sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_Z, &accel_z);

    printk("X: %.2f, Y: %.2f, Z: %.2f\n", 
        sensor_value_to_double(&accel_x),
        sensor_value_to_double(&accel_y),
        sensor_value_to_double(&accel_z));

    save_to_nvs(&accel_x); // Example of saving x-axis value
}

int main(void) {
    const struct device *dev = device_get_binding("MMA8451");

    if (!dev) {
        printk("No device found\n");
        return -1;
    }

    init_nvs();

    while (1) {
        read_sensor_data(dev);
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}