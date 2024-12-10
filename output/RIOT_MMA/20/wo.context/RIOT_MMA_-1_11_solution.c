#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"   // for delay functionality
#include "periph/i2c.h"  // I2C peripheral API
#include "mma8x5x.h"  // MMA8x5x sensor library

// Configure MMA8x5x parameters (ensure CONFIG_MMA8X5X_I2C_ADDRESS is correctly set)
#define CONFIG_MMA8X5X_I2C_ADDRESS 0x1D  // Default I2C address, change if necessary

mma8x5x_t dev;
mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    int err;
    // Initialize MMA8x5x sensor
    if ((err = mma8x5x_init(&dev, &params)) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor: %d\n", err);
        return -1;
    }

    printf("MMA8x5x sensor initialized successfully.\n");

    while (1) {
        // Read accelerometer data
        if ((err = mma8x5x_read(&dev, &data)) != MMA8X5X_OK) {
            printf("Error reading from MMA8x5x sensor: %d\n", err);
        }
        else {
            printf("Acceleration in g (x, y, z): %d, %d, %d\n", 
                   data.x_axis, data.y_axis, data.z_axis);
        }
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
