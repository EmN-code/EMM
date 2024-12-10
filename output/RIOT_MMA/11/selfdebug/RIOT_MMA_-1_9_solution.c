#include <xtimer.h>
#include <stdio.h>  // For printing output
#include <mma8x5x.h>

int main() {
    // Define parameters for the accelerometer
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS, // Use the correct macro or define it appropriately
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    mma8x5x_t dev;  // Device descriptor
    mma8x5x_data_t data;  // Data structure to hold readings

    // Initialize the MMA8x5x sensor
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x: %d\n", res);
        return 1;
    }

    while (1) {
        // Read the sensor data
        res = mma8x5x_read(&dev, &data);
        if (res != MMA8X5X_OK) {
            printf("Error reading MMA8x5x: %d\n", res);
            return 1;
        }

        // Print the data
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}