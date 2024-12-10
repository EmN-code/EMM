#include <zephyr/kernel.h>
#include <zephyr/devices.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/settings/settings.h>

#define SENSOR_LABEL "MMA8x5x"
#define NVS_STORAGE_ID 1
#define READ_INTERVAL K_SECONDS(5)

void main() {
    const struct device *dev = device_get_binding(SENSOR_LABEL);
    if (dev == NULL) {
        printk("Could not get %s device\n", SENSOR_LABEL);
        return;
    }

    struct sensor_value accel[3];
    while (1) {
        if (sensor_sample_fetch(dev) < 0) {
            printk("Sensor sample update error\n");
            continue;
        }
        
        if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
            printk("Cannot read sensor channels\n");
            continue;
        }

        printk("Accel X: %d, Y: %d, Z: %d\n", accel[0].val1, accel[1].val1, accel[2].val1);
        
        // NVS store
        settings_save_one("/sensors/last_val", accel, sizeof(accel));

        k_sleep(READ_INTERVAL);
    }
}

// Configuration for NVS
void settings_subsys_init(void) {
   settings_load();
}