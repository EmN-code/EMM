#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ztimer.h>
#include <thread.h>
#include <i2c.h>
#include <mma8x5x.h>

// Define the I2C address configuration if not defined
#ifndef CONFIG_MMA8X5X_I2C_ADDRESS
#define CONFIG_MMA8X5X_I2C_ADDRESS 0x1D // Example address
#endif

// Global device variable
dev_t dev;

int main(void) {
    printf("Starting ESP32 with MMA8x5x Sensor\n");
    
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),  // Define the I2C device used
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor!\n");
        return 1;  // Exit if failed
    }

    // Loop: Read sensor data every 5 seconds
    while (1) {
        mma8x5x_data_t data;
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Accel: X=%d Y=%d Z=%d\n", data.x, data.y, data.z);
        } else {
            printf("Error reading data!\n");
        }
        
        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5000 milliseconds
    }

    return 0;  // Should never reach here
}