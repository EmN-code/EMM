#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/types.h>
#include <sys/printk.h>

// Function declarations for clarity
void read_and_store_mma8x5x(void);

// Create a structure to hold the device information
struct device *i2c_dev;
struct device *flash_dev;

// Set the MMA8x5x I2C address and NVS parameters
#define MMA8X5X_I2C_ADDRESS 0x1D  // Modify as per your device's datasheet
#define STORAGE_PARTITION flash_area_default

// Constants
#define DELAY_MS 5000

void main(void) {
    printk("Starting MMA8x5x reader with NVS storage on ESP32\n");

    // Initialize the I2C device
    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C: Device driver not found.\n");
        return;
    }

    // Initialize the Flash device
    flash_dev = device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
    if (!flash_dev) {
        printk("Flash: Device driver not found.\n");
        return;
    }

    while (1) {
        read_and_store_mma8x5x();
        k_msleep(DELAY_MS);
    }
}

void read_and_store_mma8x5x(void) {
    uint8_t mma_data[6];
    int ret;

    // Reading data from the MMA8x5x sensor
    struct i2c_msg msgs[2];
    msgs[0].buf = mma_data;
    msgs[0].len = sizeof(mma_data);
    msgs[0].flags = I2C_MSG_WRITE;

    msgs[1].buf = mma_data;
    msgs[1].len = sizeof(mma_data);
    msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;

    ret = i2c_transfer(i2c_dev, msgs, 2, MMA8X5X_I2C_ADDRESS);
    if (ret < 0) {
        printk("Failed to read sensor data: %d\n", ret);
	} else {
		// Log and store data
		printk("Sensor data: %02x %02x %02x %02x %02x %02x\n", mma_data[0], mma_data[1], mma_data[2], mma_data[3], mma_data[4], mma_data[5]);
		// Store persistently using Flash
		ret = flash_write(flash_dev, 0x1000, mma_data, sizeof(mma_data));  // Address and size may need adjustment
		if (ret != 0) {
			printk("Failed to store data: %d\n", ret);
		}
	}
}