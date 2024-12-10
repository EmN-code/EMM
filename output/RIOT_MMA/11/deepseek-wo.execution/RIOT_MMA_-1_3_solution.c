
#include <stdio.h>
#include <unistd.h>  // For sleep function
#include "mma8x5x_params.h"
#include "mma8x5x.h"

// Define the device and data structures
static mma8x5x_t dev;
mma8x5x_data_t data;

// Configuration parameters for the MMA8x5x accelerometer
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    // Initialize the MMA8x5x accelerometer
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer: %d\n", res);
        return 1;
    }

    while (1) {
        // Read the accelerometer data
        mma8x5x_read(&dev, &data);

        // Print the accelerometer data
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);

        // Wait for 5 seconds
        sleep(5);
    }

    return 0;
}
