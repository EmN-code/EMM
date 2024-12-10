// Necessary includes in alphabetical order
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <drivers/nvs.h>
#include <sys/printk.h>
#include <storage/flash_map.h>

// Define timer delay (5 seconds)
#define SENSOR_READ_INTERVAL K_SECONDS(5)

// Function to read sensor data and store it in NVS
void sensor_read_and_store(struct device *nvs_dev, struct device *sensor_dev) {
    struct sensor_value accel_value;
    static uint16_t entry_key = 1;
    if (sensor_sample_fetch(sensor_dev) == 0) {
        if (sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, &accel_value) == 0) {
            printk("Accel: %d.%06d\n", accel_value.val1, accel_value.val2);
            nvs_write(nvs_dev, entry_key++, &accel_value, sizeof(accel_value));
            if (entry_key > 100) { // Reset key to prevent overflow
                entry_key = 1;
            }
        }
    }
}

// Main function to initialize NVS, sensor, and manage periodic reads
int main(void) {
    int err;
    struct device *nvs_dev = device_get_binding(DT_LABEL(DT_INST(0, nvs_flash_driver)));
    struct device *sensor_dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8451)));
    if (!nvs_dev || !sensor_dev) {
        printk("Failed to get device bindings\n");
        return -1;
    }
    struct nvs_fs nvs;
    nvs.sector_size = DT_N_S_sectors_SIZE(DT_INST(0, flash_map)),
    nvs.sector_count = 3;
    nvs.offset = FLASH_AREA_OFFSET(storage);
    err = nvs_init(&nvs, DT_LABEL(DT_INST(0, flash_map)));
    if (err) {
        printk("NVS initialization failed\n");
        return err;
    }
    
    print("Initialization successful, starting periodic sensor read\n");

    while (1) {
        sensor_read_and_store(nvs_dev, sensor_dev);
        k_sleep(SENSOR_READ_INTERVAL);
    }
    return 0;
}