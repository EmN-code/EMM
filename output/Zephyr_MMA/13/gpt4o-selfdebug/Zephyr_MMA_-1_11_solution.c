#include <zephyr/types.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/nvs.h>

#include <zephyr/kernel.h>

#define SLEEP_TIME_MS   5000
#define SENSOR_CHAN_DATA_CHANNEL SENSOR_CHAN_ACCEL_XYZ

void main(void)
{
    struct device *sensor_dev;
    struct sensor_value accel_data[3];
    struct flash_area *fa;
    struct nvs_fs fs;
    int rc;

    /* Bind to the MMA8x5x device */
    sensor_dev = device_get_binding("MMA8x5x");
    if (!sensor_dev) {
        printk("Sensor device MMA8x5x not found\n");
        return;
    }

    /* Initialize NVS */
    rc = flash_area_open(DT_CHOSEN_ZEPHYR_FLASH_AREA_STORAGE, &fa);
    if (rc != 0) {
        printk("Flash area open failed: %d\n", rc);
        return;
    }
    
    fs.offset = fa->fa_off;
    rc = nvs_init(&fs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
    if (rc != 0) {
        printk("NVS initialization failed: %d\n", rc);
        return;
    }

    while (1) {
        /* Fetch sensor data */
        sensor_sample_fetch(sensor_dev);
        sensor_channel_get(sensor_dev, SENSOR_CHAN_DATA_CHANNEL, accel_data);

        /* Print accelerometer data */
        printk("Accel X: %d.%06d, Y: %d.%06d, Z: %d.%06d\n",
               accel_data[0].val1, accel_data[0].val2,
               accel_data[1].val1, accel_data[1].val2,
               accel_data[2].val1, accel_data[2].val2);

        /* Store data in NVS */
        rc = nvs_write(&fs, NVS_ID, &accel_data, sizeof(accel_data));
        if (rc < 0) {
            printk("NVS write error (%d)\n", rc);
        }

        /* Sleep for a defined interval */
        k_msleep(SLEEP_TIME_MS);
    }
}