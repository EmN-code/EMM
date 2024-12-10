#include <xtimer.h>
#include <stdio.h>
#include <periph/i2c.h>
#include <periph/gpio.h>

#include "mma8x5x.h"

#define SLEEP_TIME (5U) // 5 seconds

int main(void) {
    // Initialize I2C
    i2c_t i2c = I2C_DEV(0);

    // Define parameters for MMA8x5x
    mma8x5x_params_t params = {
        .i2c = i2c,
        .addr = 0x1D, // I2C address
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize device descriptor
    mma8x5x_t dev;
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    // Main loop to read accelerometer data
    mma8x5x_data_t data;
    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Error reading data\n");
        }
        
        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_TIME);
    }

    return 0;
}