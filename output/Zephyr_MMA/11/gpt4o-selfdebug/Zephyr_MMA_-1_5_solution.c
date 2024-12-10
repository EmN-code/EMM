#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/nvs/nvs.h>
#include <zephyr/kernel.h>

#define SLEEP_TIME_MS   (5000) // Sleep time of 5 seconds

static struct nvs_fs fs;

void init_nvs() {
    int rc;
    struct flash_area *fa;

    if (flash_area_open(DT_FLASH_AREA_STORAGE_ID, &fa)) {
        printk("Can't open flash area\n");
        return;
    }

    /* Setting up file system informations */
    fs.offset = fa->fa_off;
    fs.sector_size = fa->fa_size;
    fs.sector_count = 3U;
    rc = nvs_init(&fs, DT_FLASH_AREA_STORAGE_DEV);
    if (rc) {
        printk("Flash Init failed!\n");
        return;
    }
}

void read_and_store_sensor_data() {
    const struct device *sensor = DEVICE_DT_GET_ANY(nxpsensor_mma8x5x);
    if (!device_is_ready(sensor)) {
        printk("Sensor not found\n");
        return;
    }
    struct sensor_value accel[3];

    while (1) {
        sensor_sample_fetch(sensor);
        sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel);

        printk("Accel (x, y, z): (%f, %f, %f)\n", 
               sensor_value_to_double(&accel[0]),
               sensor_value_to_double(&accel[1]),
               sensor_value_to_double(&accel[2]));

        // Store the X-axis value (as an example) in NVS
        nvs_write(&fs, 1, &accel[0].val1, sizeof(accel[0].val1));

        k_msleep(SLEEP_TIME_MS);
    }
}

int main(void) {
    printk("Starting application\n");
    init_nvs();
    read_and_store_sensor_data();
    return 0;
}