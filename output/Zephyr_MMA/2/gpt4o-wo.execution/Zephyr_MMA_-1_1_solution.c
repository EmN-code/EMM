#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/settings/nvs.h>

#define STORAGE_PARTITION_ID FIXED_PARTITION_ID(storage)
#define SENSOR_READ_INTERVAL_MS 5000
#define SENSOR_LABEL "MMA8x5x"
#define NVS_ID 1

static struct nvs_fs nvs;

void main(void) {
    const struct device *dev = device_get_binding(SENSOR_LABEL);
    if (!dev) {
        printk("Sensor device not found\n");
        return;
    }

    struct flash_area *fa;
    if (flash_area_open(STORAGE_PARTITION_ID, &fa)) {
        printk("Failed to open storage partition\n");
        return;
    }

    nvs.flash_device = fa->fa_dev;
    nvs.offset = fa->fa_off;
    nvs.sector_size = fa->fa_size;
    nvs.sector_count = fa->fa_size / fa->fa_size;

    if (nvs_init(&nvs, "")) {
        printk("NVS initialization failed\n");
        return;
    }

    struct sensor_value accel_data;
    int ret;

    while (1) {
        k_sleep(K_MSEC(SENSOR_READ_INTERVAL_MS));

        ret = sensor_sample_fetch(dev);
        if (ret) {
            printk("Failed to fetch data from sensor\n");
            continue;
        }

        ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &accel_data);
        if (ret) {
            printk("Failed to get data from sensor\n");
            continue;
        }

        printk("Acceleration: x = %d, y = %d, z = %d\n",
               sensor_value_to_double(&accel_data),
               sensor_value_to_double(&accel_data) + 1,
               sensor_value_to_double(&accel_data) + 2);

        nvs_write(&nvs, NVS_ID, &accel_data, sizeof(accel_data));
    }
    flash_area_close(fa);
}