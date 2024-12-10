// Description: The code utilizes Zephyr RTOS features to read data from an MMA8x5x accelerometer every 5 seconds on an ESP32 platform and stores the data in Non-Volatile Storage (NVS) for persistent storage. To solve the "No such file or directory" error related to 'zephyr.h', we need to ensure the correct paths are included to access the Zephyr libraries and headers.

// Imports
#include <zephyr/types.h>     // Include Zephyr specific type definitions
#include <zephyr/kernel.h>    // Include Zephyr kernel functionalities
#include <zephyr/device.h>    // Include device driver model
#include <zephyr/devicetree.h>// Include device tree macros
#include <zephyr/drivers/i2c.h> // Include I2C driver functionality
#include <zephyr/storage/flash_map.h> // Include NVS (Non-Volatile Storage) support
#include <zephyr/storage/nvs.h>    // Include NVS functionalities


#define SLEEP_TIME_MS   5000  // Define sleep time of 5 seconds

// Define the device node identifier
#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)

// Variables for devices and NVS
static const struct device *i2c_dev = DEVICE_DT_GET(MMA8X5X_NODE);
static struct nvs_fs nvs;

void main(void) {
    int err;
    uint8_t accel_data[6]; // Variable to store accelerometer readings
    uint32_t key = 1;  // Key for NVS
    
    // NVS configuration structure
    struct flash_sector fs;
    const uint32_t nwa = CONFIG_NVS_SECTOR_SIZE;

    // Ensure I2C device is ready
    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not found\n");
        return;
    }

    // Configure the NVS file system
    nvs.offset = FLASH_AREA_OFFSET(storage);
    nvs.sector_size = nwa;
    nvs.sector_count = FLASH_AREA_SIZE(storage) / nwa;
    
    err = nvs_init(&nvs, DT_LABEL(DT_CHOSEN(zephyr_flash_controller)));
    if (err) {
        printk("NVS initialization failed\n");
        return;
    }

    while (1) {
        k_msleep(SLEEP_TIME_MS);  // Sleep for defined time interval

        // Read data from MMA8x5x accelerometer
        err = i2c_read(i2c_dev, accel_data, sizeof(accel_data), MMA8X5X_NODE);
        if (err) {
            printk("Failed to read accelerometer data\n");
        } else {
            printk("Accelerometer data: %x %x\n", accel_data[0], accel_data[1]);

            // Store data in NVS
            err = nvs_write(&nvs, key, &accel_data, sizeof(accel_data));
            if (err) {
                printk("Failed to write data to NVS\n");
            }
        }
    }
}