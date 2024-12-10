#include "xtimer.h"
#include "mma8x5x.h"
#include "periph/i2c.h"
#include <stdio.h>

#define SLEEP_TIME 5

int main(void) {
    // Configure the MMA8x5x parameters
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    // Initialize the MMA8x5x device
    mma8x5x_t dev;
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x\n");
        return 1;
    }

    // Allocate memory for the data
    mma8x5x_data_t data;

    while (1) {
        // Read the accelerometer data
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        else {
            printf("Failed to read data\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_TIME);
    }

    return 0;
}