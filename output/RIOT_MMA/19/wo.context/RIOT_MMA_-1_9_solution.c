#include <stdio.h>
#include <xtimer.h>
#include <periph/i2c.h>
#include <isrpipe.h>
#include "mma8x5x.h"

int main(void) {
    printf("Initializing MMA8x5x sensor...\n");

    // I2C configuration parameters for MMA8x5x
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    mma8x5x_t dev;

    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize sensor\n");
        return -1;
    }

    printf("MMA8x5x sensor initialized successfully\n");

    // Create a loop that reads sensor data every 5 seconds
    while (1) {
        mma8x5x_data_t data;

        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Error reading sensor data\n");
        }

        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return 0;
}