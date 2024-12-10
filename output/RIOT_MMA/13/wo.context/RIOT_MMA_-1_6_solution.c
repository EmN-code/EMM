#include <stdio.h>
#include <stdlib.h>
#include "mma8x5x.h"
#include "xtimer.h"
#include "thread.h"

#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static mma8x5x_t dev;

int main(void) {
    // Setting up parameters for the MMA8x5x accelerometer
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed!");
        return 1;
    }
    puts("MMA8x5x initialized successfully.");

    // Infinite loop to read data every 5 seconds
    while (1) {
        mma8x5x_data_t data;
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            puts("Read failed!");
        }

        xtimer_sleep(INTERVAL / 1000000U); // Delay for 5 seconds
    }
    return 0;
}