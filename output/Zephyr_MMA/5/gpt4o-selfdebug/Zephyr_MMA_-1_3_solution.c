#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/data/nvs.h>  // Correct header file for NVS in Zephyr

#define SENSOR_NODE DT_ALIAS(mma8x5x)
#define READ_INTERVAL K_SECONDS(5)

void main(void) {
    const struct device *sensor = DEVICE_DT_GET(SENSOR_NODE);
    if (!device_is_ready(sensor)) {
        printk("Sensor not ready\n");
        return;
    }

    struct sensor_value value;
    int err;

    // Initialize the NVS file system
    static struct nvs_fs fs;
    struct flash_pages_info info;

    fs.sector_size = 4096;
    fs.sector_count = 3;
    fs.offset = DT_REG_ADDR(DT_NODELABEL(storage));  // Make sure this line points to your storage label
    err = nvs_mount(&fs);
    if (err) {
        printk("Failed to mount NVS\n");
        return;
    }

    while (1) {
        err = sensor_sample_fetch(sensor);
        if (err) {
            printk("Failed to fetch sensor data\n");
        } else {
            sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, &value);
            printk("AccValue: %d.%06d\n", value.val1, value.val2);
            
            // Save the reading to NVS
            err = nvs_write(&fs, CONFIG_NVS_ID, &value, sizeof(value));
            if (err < 0) {
                printk("Failed to write to NVS\n");
            } else {
                printk("Reading stored in NVS\n");
            }
        }
        k_sleep(READ_INTERVAL);
    }
}