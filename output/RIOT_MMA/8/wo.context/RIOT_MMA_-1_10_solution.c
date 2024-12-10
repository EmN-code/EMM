#include <xtimer.h> // For the xtimer_sleep function
#include <stdio.h> // For standard input output
#include <stdint.h> // For standard integer types
#include <string.h> // For string manipulation
#include "mma8x5x.h" // Include MMA8x5x headers
#include "periph/i2c.h" // Include peripheral I2C for ESP32

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    // MMA8X5X parameters
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS, // Correctly define the I2C address
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G,
        .offset = {0, 0, 0}
    };

    // Initialize the MMA8x5x device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer\n");
        return 1;
    }

    while (1) {
        // Read the data from the accelerometer
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            printf("Error reading accelerometer data\n");
        } else {
            printf("X: %d, Y: %d, Z: %d\n",
                   data.x, data.y, data.z);
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}