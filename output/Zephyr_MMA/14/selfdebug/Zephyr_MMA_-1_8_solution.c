
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define FLASH_NODE DT_NODELABEL(flash_controller)

#define FLASH_PARTITION_ID FIXED_PARTITION_ID(storage)
#define FLASH_PARTITION DT_LABEL(DT_NODELABEL(storage))

#define SLEEP_TIME_MS 5000

const struct device *flash_dev;
const struct device *sensor_dev;

void read_sensor_and_store(void)
{
    struct sensor_value accel[3];
    int rc;

    if (!device_is_ready(sensor_dev)) {
        printk("Sensor device not ready\n");
        return;
    }

    rc = sensor_sample_fetch(sensor_dev);
    if (rc) {
        printk("Sensor sample fetch failed: %d\n", rc);
        return;
    }

    rc = sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (rc) {
        printk("Sensor channel get failed: %d\n", rc);
        return;
    }

    printk("Accel: x=%f, y=%f, z=%f\n", sensor_value_to_double(&accel[0]),
           sensor_value_to_double(&accel[1]), sensor_value_to_double(&accel[2]));

    // Store the read values in NVS
    off_t offset = 0;
    size_t size = sizeof(accel);

    if (!device_is_ready(flash_dev)) {
        printk("Flash device not ready\n");
        return;
    }

    rc = flash_erase(flash_dev, offset, size);
    if (rc) {
        printk("Flash erase failed: %d\n", rc);
        return;
    }

    rc = flash_write(flash_dev, offset, accel, size);
    if (rc) {
        printk("Flash write failed: %d\n", rc);
        return;
    }

    printk("Data stored in NVS\n");
}

int main(void)
{
    sensor_dev = DEVICE_DT_GET(MMA8X5X_NODE);
    flash_dev = FLASH_DEVICE_DT_GET(FLASH_NODE);

    while (1) {
        read_sensor_and_store();
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
