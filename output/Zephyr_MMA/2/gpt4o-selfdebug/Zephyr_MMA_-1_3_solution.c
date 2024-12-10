#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/nvs/nvs.h>

#define SLEEP_TIME_MS 5000
#define STORAGE_ID 1

void read_and_store_sensor_value(const struct device *sensor, struct nvs_fs *nvs)
{
    struct sensor_value accel_value;

    if (sensor_sample_fetch(sensor) < 0) {
        printk("Failed to fetch sample\n");
        return;
    }

    if (sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, &accel_value) < 0) {
        printk("Failed to get sensor data\n");
        return;
    }

    printk("Acceleration: %d\n", accel_value.val1);

    if (nvs_write(nvs, STORAGE_ID, &accel_value.val1, sizeof(accel_value.val1)) < 0) {
        printk("Failed to write to NVS\n");
    } else {
        printk("Value saved to NVS\n");
    }
}

int main(void)
{
    const struct device *sensor = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8652fc)));
    if (!sensor) {
        printk("Failed to get sensor device\n");
        return -1;
    }

    struct nvs_fs nvs;
    struct flash_sector fs_sectors;
    nvs.sector_size = FLASH_AREA_SIZE(storage_partition);
    nvs.sector_count = 1;
    nvs.offset = FLASH_AREA_OFFSET(storage_partition);

    if (flash_area_get_sectors(DT_FIXED_PARTITION_ID(storage_partition), &fs_sectors, NULL) != 0) {
        printk("Failed to get flash sector\n");
        return -1;
    }

    nvs.sector_size = fs_sectors.fs_size;
    nvs.sector_count = 1;
    nvs.offset = fs_sectors.fs_off;

    int rc = nvs_init(&nvs, DT_CHOSEN(zephyr_flash_controller));
    if (rc) {
        printk("NVS init failed\n");
        return rc;
    }

    while (1) {
        read_and_store_sensor_value(sensor, &nvs);
        k_msleep(SLEEP_TIME_MS);
    }

    return 0;
}