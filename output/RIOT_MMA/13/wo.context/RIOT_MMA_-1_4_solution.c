#include "xtimer.h"
#include "mma8x5x.h"
#include <stdio.h>

// Define the I2C address for the MMA8x5x
#define MMA8X5X_I2C_ADDRESS (0x1D)

// Create a device descriptor
static mma8x5x_t dev;

// Initialize parameters with the correct address
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_and_print_mma8x5x(void) {
    mma8x5x_data_t data;
    int res = mma8x5x_read(&dev, &data);
    if (res == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read from MMA8x5x: %d\n", res);
    }
}

int main(void) {
    // Initialize the sensor
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Initialization failed: %d\n", res);
        return 1;
    }

    // Continuously read data every 5 seconds
    while (1) {
        read_and_print_mma8x5x();
        xtimer_sleep(5);
    }
    return 0;
}