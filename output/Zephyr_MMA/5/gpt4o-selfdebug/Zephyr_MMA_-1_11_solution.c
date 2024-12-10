#include <zephyr/types.h>  // Ensure correct system types are included
#include <zephyr/syscalls/flash.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/nvs/nvs.h>

// Sensor and storage configurations
#define SLEEP_TIME_MS   5000
#define STORAGE_ID     1

void main(void) {
    struct device *dev;
    struct sensor_value temp_value;
    int rc;

    // Initialize the Non-Volatile Storage
    struct nvs_fs fs;
    struct flash_sector sectors[10];
    const struct flash_area *fa;

    rc = flash_area_open(STORAGE_ID, &fa);
    if (rc < 0) {
        printk("Flash area open error\n");
        return;
    }

    rc = flash_area_get_sectors(STORAGE_ID, &sectors);
    if (rc < 0) {
        printk("Unable to get flash sectors\n");
        return;
    }

    fs.flash_device = FLASH_AREA_DEVICE(fa);
    fs.offset = fa->fa_off;
    fs.sector_count = 10;
    fs.sector_size = fa->fa_size / 10;

    rc = nvs_init(&fs, &fs.flash_device);
    if (rc) {
        printk("NVS initialization failed\n");
        return;
    }

    // Initialize the sensor device
    dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8x5x)));
    if (!dev) {
        printk("No device found.\n");
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(dev);
        if (rc) {
            printk("Sensor sample update error\n");
            return;
        }

        rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &temp_value);
        if (rc) {
            printk("Cannot read sensor channel\n");
            return;
        }

        printk("X=%d, Y=%d, Z=%d\n", temp_value.val1, temp_value.val1, temp_value.val1);

        // Persistently store the reading
        rc = nvs_write(&fs, STORAGE_ID, &temp_value, sizeof(temp_value));
        if (rc < 0) {
            printk("Failed to write to storage\n");
        }

        k_msleep(SLEEP_TIME_MS);
    }
}

// Ensure your CMakeLists.txt or prj.conf has CONFIG_NVS=y and CONFIG_FLASH=y enabled. This ensures that NVS and flash storage capabilities are built into the project.