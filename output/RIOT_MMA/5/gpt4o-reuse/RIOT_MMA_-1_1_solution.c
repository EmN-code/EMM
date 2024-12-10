
#include <stdio.h>
#include <xtimer.h>
#include "mma8x5x.h"

#define SLEEP_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0), // Assuming I2C device 0 is used
        .addr = MMA8X5X_I2C_ADDRESS, // Default I2C address for MMA8x5x
        .mode = MMA8X5X_MODE_NORMAL,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x sensor failed");
        return 1;
    }

    while (1) {
        int16_t x, y, z;

        // Read acceleration data
        if (mma8x5x_read(&dev, &x, &y, &z) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", x, y, z);
        } else {
            puts("Failed to read acceleration data");
        }

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
