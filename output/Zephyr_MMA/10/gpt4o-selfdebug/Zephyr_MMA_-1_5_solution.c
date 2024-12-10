#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/nvs.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>

#define SENSOR_DEV_NAME DT_LABEL(DT_ALIAS(mma8x5x))
#define STORAGE_PARTITION_LABEL storage

struct nvs_fs nvs;
static int8_t sensor_reading = 0;

void sensor_reading_thread(void)
{
    const struct device *sensor = device_get_binding(SENSOR_DEV_NAME);
    if (!sensor) {
        printk("No device found.");
        return;
    }

    struct sensor_value temp_value;

    while (1) {
        if (sensor_sample_fetch(sensor) < 0) {
            printk("Sensor fetch failed.");
        } else {
            if (sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, &temp_value) == 0) {
                sensor_reading = temp_value.val1;
                // Store to NVS
                int rc = nvs_write(&nvs, sizeof(sensor_reading), &sensor_reading, sizeof(sensor_reading));
                if (rc < 0) {
                    printk("Error writing to NVS: %d", rc);
                }
            }
        }
        k_sleep(K_SECONDS(5));
    }
}

int main(void)
{
    int rc;
    const struct flash_area *fa;

    rc = flash_area_open(DT_FLASH_AREA_STORAGE_ID, &fa);
    if (rc < 0) {
        printk("Flash area open failed.");
        return rc;
    }

    nvs.offset = fa->fa_off;
    nvs.sector_size = fa->fa_size;
    nvs.sector_count = 3;
    rc = nvs_init(&nvs, STORAGE_PARTITION_LABEL);
    if (rc < 0) {
        printk("NVS Initialization failed.");
        return rc;
    }

    // Create the thread to read sensor values
    k_thread_create(&sensor_thread_data, sensor_thread_stack, K_THREAD_STACK_SIZEOF(sensor_thread_stack),
                    (k_thread_entry_t)sensor_reading_thread, NULL, NULL, NULL, K_PRIO_PREEMPT(5), 0, K_NO_WAIT);

    return 0;
}