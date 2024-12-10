// The error indicates that the path to the NVS header file is incorrect. We need to include the correct path for the NVS header file.

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/nvs/nvs.h> // Correcting the path of NVS header file

// Define the interval for reading the sensor in milliseconds
#define SENSOR_READ_INTERVAL K_SECONDS(5)

// Define the NVS file system storage
static struct nvs_fs fs;

// Function to read sensor data
static void read_mma8x5x_sensor(void) {
    const struct device *sensor_dev = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8x5x_mma8652fc)));
    if (sensor_dev == NULL) {
        printk("Could not get MMA8x5x device\n");
        return;
    }

    struct sensor_value accel_x, accel_y, accel_z;
    if (sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, &accel_x) < 0) {
        printk("Failed to fetch sensor data\n");
        return;
    }

    // Print the accelerometer reading
    printk("Accel X: %d, Y: %d, Z: %d\n", sensor_value_to_double(&accel_x), 
                                                  sensor_value_to_double(&accel_y), 
                                                  sensor_value_to_double(&accel_z));

    // Write data to NVS
    int err = nvs_write(&fs, 1, &accel_x, sizeof(accel_x));
    if (err < 0) {
        printk("Failed to store data in NVS\n");
    }
}

void main(void) {
    // Initialize NVS file system
    struct flash_area *fa;
    int rc = flash_area_open(NVS_SMALL_FILE_STORAGE_ID, &fa);
    if (rc < 0) {
        printk("Failed to open flash area\n");
        return;
    }

    fs.offset = fa->fa_off;
    fs.sector_size = 4096;
    fs.sector_count = 3;

    rc = nvs_init(&fs, fa->fa_dev);
    if (rc) {
        printk("Failed to initialize NVS\n");
        return;
    }

    // Main execution loop
    while (1) {
        read_mma8x5x_sensor();
        k_sleep(SENSOR_READ_INTERVAL);
    }
}