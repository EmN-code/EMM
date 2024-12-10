#include <xtimer.h>  // For the xtimer_sleep function to control the read interval
#include <stdio.h>   // For the printf function for console output
#include <stdbool.h> // For using boolean types
#include "mma8x5x.h"  // Include the MMA8x5x driver header

// Define the I2C device and address according to your specific setup, typically set for I2C_DEV(0)
#define I2C_DEVICE I2C_DEV(0)
#define MMA8X5X_I2C_ADDRESS 0x1D // Default I2C address for MMA8x5x

// Function to initialize and configure the MMA8x5x sensor
event_queue_t event_queue;
mma8x5x_t dev;
mma8x5x_data_t data;

void init_mma8x5x(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEVICE,
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
    }
}

// Main function
int main(void) {
    // Initialize the MMA8x5x sensor
    init_mma8x5x();

    while (1) {
        // Read accelerometer data
        mma8x5x_read(&dev, &data);
        printf("Acceleration X: %d, Y: %d, Z: %d\n", data.accel[0], data.accel[1], data.accel[2]);
        
        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}

