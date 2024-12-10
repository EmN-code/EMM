// Include required header files in descending alphabetical order
#include <xtimer.h>
#include <stdio.h>
#include "periph/i2c.h"
#include "mma8x5x.h"

// Main function
int main(void) {
    // Initialize MMA8x5x sensor parameters
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Declare MMA8x5x device and data structures
    mma8x5x_t dev;
    mma8x5x_data_t data;

    // Initialize the MMA8x5x device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return -1;
    }

    // Continuous loop to read accelerometer data every 5 seconds
    while(1) {
        // Read accelerometer data
        mma8x5x_read(&dev, &data);

        // Print the acceleration data
        printf("Acceleration - X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}