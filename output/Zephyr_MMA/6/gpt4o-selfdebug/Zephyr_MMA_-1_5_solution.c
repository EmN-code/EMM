#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define STORAGE_PARTITION flash_area_id_from_devfs_stm(
        DT_CHOSEN(zephyr_storage_partition))
#define STORAGE_FLASH_AREA "storage"

#define SLEEP_TIME_MS   5000

void main(void) {
    const struct device *sensor = device_get_binding("MMA8x5x");
    if (sensor == NULL) {
        printk("Failed to get device binding\n");
        return;
    }

    struct sensor_value accel[3];
    struct nvs_fs fs;
    int err;

    fs.offset = FLASH_AREA_OFFSET(STORAGE_PARTITION);
    fs.sector_size = FLASH_AREA_SIZE(STORAGE_PARTITION);
    fs.sector_count = 3U;

    err = nvs_init(&fs, STORAGE_FLASH_AREA);
    if (err) {
        printk("Flash Init failed\n");
        return;
    }

    while (1) {
        err = sensor_sample_fetch(sensor);
        if (err) {
            printk("Sensor sample update error\n");
            continue;
        }

        err = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_X, &accel[0]);
        err = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_Y, &accel[1]);
        err = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_Z, &accel[2]);
        if (err) {
            printk("Cannot read sensor channels\n");
            continue;
        }

        printk("Accel X: %d.%06d \t Y: %d.%06d \t Z: %d.%06d\n",
                accel[0].val1, accel[0].val2,
                accel[1].val1, accel[1].val2,
                accel[2].val1, accel[2].val2);

        err = nvs_write(&fs, 1, &accel, sizeof(accel));
        if (err < 0) {
            printk("Write failed\n");
        }

        k_msleep(SLEEP_TIME_MS);
    }
}