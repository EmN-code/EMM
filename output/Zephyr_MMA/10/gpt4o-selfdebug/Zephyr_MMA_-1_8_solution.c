#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/nvs.h>
#include <zephyr/settings/settings.h>

#define MMA8x5x_LABEL "MMA8x5x"
#define SLEEP_TIME_MS   K_SECONDS(5)
#define STORAGE_ID 1 // Change ID accordingly to what NVS is configured

void main(void)
{
    const struct device *dev = device_get_binding(MMA8x5x_LABEL);
    if (dev == NULL) {
        printk("Device not found\n");
        return;
    }

    struct nvs_fs nvs;
    int rc;
    struct sensor_value mma_value;

    nvs.offset = STORAGE_ID;
    rc = nvs_init(&nvs, "storage");
    if (rc) {
        printk("NVS init failed\n");
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(dev);
        if (rc) {
            printk("Fetch sample failed\n");
            return;
        }

        rc = sensor_channel_get(dev, SENSOR_CHAN_ALL, &mma_value);
        if (rc) {
            printk("Sensor channel get failed\n");
            return;
        }

        // Store sensor value persistently
        rc = nvs_write(&nvs, STORAGE_ID, &mma_value, sizeof(mma_value));
        if (rc > 0) {
            printk("Stored value: %d\n", mma_value.val1);
        } else {
            printk("NVS write failed\n");
        }

        k_sleep(SLEEP_TIME_MS);
    }
}