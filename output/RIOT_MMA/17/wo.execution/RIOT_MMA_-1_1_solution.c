#include <thread_arch.h>  // Provides threading architecture in RIOT
#include <stdio.h>  // For basic input and output functions
#include <stdlib.h>  // Standard library for memory allocation and control
#include <periph_conf.h>  // Configuration for peripheral device drivers
#include "mma8x5x.h"  // Include the MMA8x5x driver header

// Define parameters for the MMA8x5x sensor
static const mma8x5x_params_t mma_params = {
    .i2c = I2C_DEV(0),  // Define which I2C device to use
    .addr = 0x1D        // I2C address for the MMA8x5x sensor
};

// Create an MMA8x5x device descriptor
static mma8x5x_t dev;

// Function to initialize the MMA8x5x sensor
void init_mma8x5x(void) {
    if (mma8x5x_init(&dev, &mma_params) != 0) {
        printf("Error: Couldn't initialize MMA8x5x sensor\n");
        exit(EXIT_FAILURE);
    } else {
        printf("MMA8x5x initialized successfully\n");
    }
}

// Function to read data from the sensor
void read_mma8x5x(void) {
    if (mma8x5x_is_ready(&dev)) {
        uint16_t x, y, z;
        mma8x5x_read(&dev, &x, &y, &z);
        printf("MMA8x5x Readings - X: %u, Y: %u, Z: %u\n", x, y, z);
    } else {
        printf("MMA8x5x is not ready\n");
    }
}

int main(void) {
    // Initialize the MMA8x5x sensor
    init_mma8x5x();

    // Create a thread to read MMA8x5x sensor data every 5 seconds
    while (1) {
        read_mma8x5x();
        thread_sleep(5);
    }

    return 0;
}