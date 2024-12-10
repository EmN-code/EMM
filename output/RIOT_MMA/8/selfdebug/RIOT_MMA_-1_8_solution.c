#include "periph/i2c.h"
#include "xtimer.h"
#include "mma8x5x.h"

// Define the MMA8x5x device struct and parameter configurations
static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),    // use I2C device 0
    .addr = 0x1D,        // standard address for MMA8x5x
    .rate = MMA8X5X_RATE_50HZ, // Sample rate
    .range = MMA8X5X_RANGE_2G  // Range
};

int main(void) {
    int res;

    // Initialize the device
    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x, error code: %d\n", res);
        return -1;
    }

    printf("MMA8x5x initialized successfully\n");

    mma8x5x_data_t data; // Variable to store the sensor data

    while (1) {
        // Read data from the sensor
        res = mma8x5x_read(&dev, &data);
        if (res != 0) {
            printf("Failed to read from MMA8x5x, error code: %d\n", res);
        } else {
            printf("X=%.2fG, Y=%.2fG, Z=%.2fG\n", data.x_axis_g, data.y_axis_g, data.z_axis_g);
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}