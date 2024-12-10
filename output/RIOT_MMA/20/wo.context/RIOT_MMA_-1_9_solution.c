#include "periph/i2c.h"
#include "xtimer.h"
#include <stdio.h>

// Define configuration for MMA8x5x device
#define I2C_DEV_ID           (0)    // I2C device index
#define MMA8X5X_ADDR         (0x1D) // Default I2C address for MMA8653
#define MMA8X5X_RATE         (MMA8X5X_RATE_50HZ)
#define MMA8X5X_RANGE        (MMA8X5X_RANGE_2G)
#define READ_INTERVAL_SEC    (5)    // Interval to read the sensor data

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    // Set up parameters for the accelerometer
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(I2C_DEV_ID),
        .addr = MMA8X5X_ADDR,
        .rate = MMA8X5X_RATE,
        .range = MMA8X5X_RANGE,
    };

    // Initialize the MMA8x5x device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return -1;
    }

    printf("MMA8x5x initialized successfully\n");

    while (1) {
        // Read data from the MMA8x5x accelerometer
        mma8x5x_read(&dev, &data);

        // Print the accelerometer data
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);

        // Wait for the specified interval
        xtimer_sleep(READ_INTERVAL_SEC);
    }

    return 0;
}