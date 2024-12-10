// Description
// This code sets up an ESP32 using RIOT to interface with an MMA8x5x accelerometer. It reads data every 5 seconds and prints it. 
// It initializes the I2C communication and the accelerometer with specified parameters. 

// Imports
#include <xtimer.h>
#include <stdio.h>
#include <periph/i2c.h>
#include <mma8x5x.h>

// Function to read data from the accelerometer
tatic void read_accelerometer(mma8x5x_t *dev) {
    mma8x5x_data_t data;
    if (mma8x5x_read(dev, &data) == MMA8X5X_OK) {
        printf("Acceleration: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        printf("Error reading data\n");
    }
}

int main(void) {
    // Initialize the I2C interface
    i2c_init(I2C_DEV(0));

    // Set up accelerometer parameters
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Declare a device descriptor
    mma8x5x_t dev;

    // Initialize the accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed");
        return 1;
    }

    while (1) {
        // Read and print accelerometer data
        read_accelerometer(&dev);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}