#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define SLEEP_TIME_S 5

void read_and_store_sensor_data(const struct device *sensor, struct nvs_fs *fs) {
    struct sensor_value accel[3];
    int rc = sensor_sample_fetch(sensor);
    if (rc == 0) {
        rc = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel);
    }

    if (rc == 0) {
        printk("Acceleration X: %d.%06d, Y: %d.%06d, Z: %d.%06d\n",
               accel[0].val1, accel[0].val2,
               accel[1].val1, accel[1].val2,
               accel[2].val1, accel[2].val2);

        /* Store data persistently using NVS */
        struct nvs_fs temp_fs = *fs; // Create a local copy to avoid writing the same data twice
        rc = nvs_write(&temp_fs, 1, &accel, sizeof(accel));

        if (rc >= 0) {
            printk("Sensor data stored successfully!\n");
        } else {
            printk("Error storing sensor data: %d\n", rc);
        }
    } else {
        printk("Sensor sample fetch or channel get error: %d\n", rc);
    }
}

int main(void) {
    const struct device *sensor = device_get_binding("MMA8x5x");

    if (!sensor) {
        printk("Could not get MMA8x5x device\n");
        return -1;
    }

    /* Setup non-volatile storage (NVS) file system */
    struct nvs_fs fs;
    fs.offset = FLASH_AREA_OFFSET(storage);
    fs.sector_size = FLASH_AREA_SIZE(storage);
    fs.sector_count = 3U; // Use 3 sectors in the storage flash area

    int rc = nvs_mount(&fs);
    if (rc) {
        printk("Flash Init failed\n");
        return rc;
    }

    printk("Starting the application\n");

    while (1) {
        read_and_store_sensor_data(sensor, &fs);
        k_sleep(K_SECONDS(SLEEP_TIME_S));
    }

    return 0;
}
