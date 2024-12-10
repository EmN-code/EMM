#include <stdio.h>
#include "xtimer.h"
#include "mma8x5x.h"
#include "periph/i2c.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    // Initialize I2C interface
    i2c_init(I2C_DEV(0));

    // Device parameters
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8x5X_I2C_ADDRESS,
        .rate = MMA8x5X_RATE_50HZ,
        .range = MMA8x5X_RANGE_2G
    };

    // Initialize the device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error initializing sensor\n");
        return 1;
    }

    while (1) {
        // Read the accelerometer data
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %f, Y: %f, Z: %f\n", data.x, data.y, data.z);
        } else {
            printf("Error reading sensor data\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}