#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/flash_map_defs.h>
#include <zephyr/nvs/nvs.h>

#define STORAGE_PARTITION_LABEL storage
#define MMA8X5X_DEVICE_LABEL "MMA8X5X"

void main(void) {
    const struct device *mma8x5x = device_get_binding(MMA8X5X_DEVICE_LABEL);
    if (!mma8x5x) {
        printk("No MMA8x5x device found\n");
        return;
    }

    /* NVS file system initialization */
    static struct flash_area *fa;
    int rc;

    rc = flash_area_open(FLASH_AREA_ID(STORAGE_PARTITION_LABEL), &fa);
    if (rc) {
        printk("Failed to open flash area\n");
        return;
    }

    struct nvs_fs fs;
    fs.offset = fa->fa_offset;
    fs.sector_size = fa->fa_size;
    fs.sector_count = 3U;

    rc = nvs_init(&fs, STORAGE_PARTITION_LABEL);
    if (rc) {
        printk("Failed to initialize NVS\n");
        return;
    }

    while (1) {
        struct sensor_value accel[3];

        if (sensor_channel_get(mma8x5x, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
            printk("Failed to get sensor data\n");
        } else {
            /* Store data in NVS */
            off_t offset = 0;
            rc = nvs_write(&fs, offset, &accel, sizeof(accel));
            if (rc < 0) {
                printk("Failed to write data to NVS\n");
            } else {
                printk("Data stored successfully\n");
            }
        }
        k_sleep(K_SECONDS(5));
    }
}