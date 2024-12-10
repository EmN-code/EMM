#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/storage/flash_map.h>

void main(void) {
    const struct device *dev = device_get_binding("MMA8x5x");
    if (!dev) {
        printk("Failed to get binding for MMA8x5x sensor\n");
        return;
    }

    while (1) {
        int accel_reading = 0; 
        int rc = sensor_sample_fetch(dev);
        if (rc == 0) {
            struct sensor_value accel_val;
            sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &accel_val);
            accel_reading = accel_val.val1; 
            printk("Accelerometer reading: %d\n", accel_reading);

            /* Store reading to NVS */
            const struct flash_area *fa;
            flash_area_open(DT_FLASH_AREA_STORAGE_ID, &fa);
            flash_area_erase(fa, 0, fa->fa_size);
            flash_area_write(fa, 0, &accel_reading, sizeof(accel_reading));
            flash_area_close(fa);
        } else {
            printk("Failed to fetch sensor sample\n");
        }
        k_sleep(K_SECONDS(5));
    }
}