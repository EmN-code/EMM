#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/nvs/nvs.h>
#include <zephyr/kernel.h>

#define STORAGE_ID 1

void read_and_store_sensor_data(const struct device *sensor, const struct nvs_fs *fs)
{
    /* Variable to hold sensor value */
    struct sensor_value accel;
    int rc;

    /* Fetch sensor data */
    rc = sensor_sample_fetch(sensor);
    if (rc == 0) {
        rc = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, &accel);
        if (rc == 0) {
            printk("Accel: %d.%06d\n", accel.val1, accel.val2);
            
            /* Store value in NVS */
            nvs_write(fs, STORAGE_ID, &accel, sizeof(accel));
        }
    }
    if (rc != 0) {
        printk("Error reading from MMA8x5x\n");
    }
}

int main(void)
{
    const struct device *sensor;
    struct nvs_fs fs;

    /* Configure sensor device */
    sensor = device_get_binding("MMA8X5X");
    if (!sensor) {
        printk("Could not find MMA8x5x device\n");
        return -1;
    }

    /* Configure NVS */
    fs.sector_size = FLASH_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;
    nvs_init(&fs);

    /* Loop to read data every 5 seconds */
    while (1) {
        read_and_store_sensor_data(sensor, &fs);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
