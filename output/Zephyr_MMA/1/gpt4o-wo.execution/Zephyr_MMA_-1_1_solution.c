#include <zephyr.h>
#include <drivers/sensor.h>
#include <device.h>
#include <drivers/flash.h>
#include <storage/flash_map.h>
#include <storage/flash_area.h>
#include <nvs/nvs.h>

#define SLEEP_TIME_MS 5000

/* Define the NVS key for storing sensor data */
#define SENSOR_DATA_ID 1

void main(void) {
    const struct device *sensor_dev;
    struct sensor_value accel[3];
    struct nvs_fs fs;
    int ret;
    u32_t stored_val;

    /* Initialize the flash storage */
    fs.offset = FLASH_AREA_OFFSET(storage);
    fs.sector_size = FLASH_AREA_SIZE(storage);
    fs.sector_count = 3;  // Set according to your requirements

    ret = nvs_mount(&fs);
    if (ret) {
        printk("NVS mount failed: %d\n", ret);
        return;
    }

    /* Get the device binding for the MMA8x5x sensor */
    sensor_dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8x5x)));
    if (!sensor_dev) {
        printk("Failed to get sensor device\n");
        return;
    }

    printk("Successfully got the MMA8x5x sensor\n");

    while (1) {
        /* Fetch the sensor data */
        ret = sensor_sample_fetch(sensor_dev);
        if (ret) {
            printk("Failed to fetch sensor sample: %d\n", ret);
            k_sleep(K_MSEC(SLEEP_TIME_MS));
            continue;
        }

        /* Get accelerometer values */
        ret = sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (ret) {
            printk("Failed to get sensor data: %d\n", ret);
            k_sleep(K_MSEC(SLEEP_TIME_MS));
            continue;
        }

        printk("Accel X: %d, Y: %d, Z: %d\n", accel[0].val1, accel[1].val1, accel[2].val1);

        /* Store the X-axis reading persistently in NVS */
        ret = nvs_write(&fs, SENSOR_DATA_ID, &accel[0].val1, sizeof(accel[0].val1));
        if (ret < 0) {
            printk("Failed to write to NVS: %d\n", ret);
            k_sleep(K_MSEC(SLEEP_TIME_MS));
            continue;
        }

        /* Optionally read back the stored value to verify NVS write */
        ret = nvs_read(&fs, SENSOR_DATA_ID, &stored_val, sizeof(stored_val));
        if (ret > 0) {
            printk("Stored Accel X: %d\n", stored_val);
        }

        /* Sleep for the defined interval */
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}