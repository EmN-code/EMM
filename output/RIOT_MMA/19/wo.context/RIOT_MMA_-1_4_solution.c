#include <xtimer.h>
#include <stdio.h>
#include <periph/i2c.h>
#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = 0x1D, // Default I2C address for MMA8653
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    // Initialize the I2C interface
    if (i2c_init(I2C_DEV(0)) != 0) {
        printf("Error initializing I2C\n");
        return 1;
    }

    // Initialize the accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x\n");
        return 1;
    }

    // Continuous loop to read data every 5 seconds
    while (1) {
        mma8x5x_data_t data;

        // Read accelerometer data
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            printf("Error reading accelerometer data\n");
            continue;
        }

        // Assuming correct field names based on mma8x5x_data_t structure
        printf("Acceleration Data: X=%d Y=%d Z=%d\n", data.x, data.y, data.z);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}