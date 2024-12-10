#include <xtimer.h>            // Include for timer functionalities
#include <ztimer.h>            // Necessary for ztimer callback structures
#include <stdlib.h>            // Include for standard library functions
#include <stdio.h>             // Include for input-output functions
#include <mma8x5x.h>           // Header for MMA8x5x sensor functionality
#include <kernel.h>            // Essential RIOT kernel functions

#define INTERVAL (5U * 1000000U) // 5 seconds interval, in microseconds

static mma8x5x_t dev; // Create a device descriptor for the accelerometer

// Function that initializes the MMA8x5x accelerometer
void init_mma8x5x(mma8x5x_t *dev) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(dev, &params) != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x sensor\n");
    }
}

// Main function implementing the periodic sensor read
int main(void) {
    init_mma8x5x(&dev);  // Initialize the accelerometer
    mma8x5x_data_t data; // Structure to hold the sensor readings

    while (1) {
        // Read sensor data
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Error reading data\n");
        }
        
        // Wait for next interval
        xtimer_usleep(INTERVAL);
    }
    return 0;
}