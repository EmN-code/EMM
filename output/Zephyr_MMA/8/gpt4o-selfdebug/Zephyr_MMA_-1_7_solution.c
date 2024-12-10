#include <zephyr/sys/mutex.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/flash.h>
#include <zephyr/settings/settings.h>

#define SENSOR_READ_INTERVAL K_SECONDS(5)

void main(void) {
    const struct device *sensor_device;
    struct sensor_value temperature;
    int ret;

    sensor_device = device_get_binding("MMA8x5x");
    if (!sensor_device) {
        printk("Sensor device not found\n");
        return;
    }

    printf("Sensor device found\n");

    ret = settings_subsys_init();
    if (ret) {
        printk("Settings subsystem initialization failed\n");
        return;
    }

    while (1) {
        ret = sensor_sample_fetch(sensor_device);
        if (ret) {
            printk("Error fetching sensor sample\n");
            continue;
        }

        ret = sensor_channel_get(sensor_device, SENSOR_CHAN_ALL, &temperature);
        if (ret) {
            printk("Error getting sensor channel data\n");
            continue;
        }

        printf("Temperature: %d\n", sensor_value_to_double(&temperature));

        // Use NVS to store temperature persistently
        // This part assumes setup and implementation of NVS storage
        
        k_sleep(SENSOR_READ_INTERVAL);
    }
}