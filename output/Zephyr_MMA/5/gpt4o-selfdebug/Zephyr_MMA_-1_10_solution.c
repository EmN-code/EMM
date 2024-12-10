#include <zephyr/syscalls/flash.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/nvs/nvs.h>

#define STORAGE_ID 1
#define SLEEP_TIME K_SECONDS(5)

void main(void) {
    const struct device *dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8652fcm)));
    if (!dev) {
        printk("Failed to get device binding\n");
        return;
    }

    struct sensor_value accel[3];

    int rc = nvs_init(&fs, FLASH_AREA_ID(storage), fs_storage, fs_storage->size);
    if (rc) {
        printk("NVS initialization failed\n");
        return;
    }

    while (1) {
        sensor_sample_fetch(dev);
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_X, &accel[0]);
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Y, &accel[1]);
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Z, &accel[2]);

        printk("Acceleration - X: %d, Y: %d, Z: %d\n", accel[0].val1, accel[1].val1, accel[2].val1);

        rc = nvs_write(&fs, STORAGE_ID, &accel, sizeof(accel));
        if (rc < 0) {
            printk("Error writing to NVS\n");
        }

        k_sleep(SLEEP_TIME);
    }
}