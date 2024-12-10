#include <stdio.h>
#include <periph/i2c.h>
#include <mma8x5x.h>
#include "xtimer.h"

static mma8x5x_t dev;
mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    // Initialize the MMA8x5x accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error: Unable to initialize accelerometer\n");
        return 1;
    }

    printf("Accelerometer initialized successfully!\n");

    while (1) {
        // Read data from the accelerometer
        mma8x5x_read(&dev, &data);
        printf("Acceleration data - X: %d mg, Y: %d mg, Z: %d mg\n", data.x_axis, data.y_axis, data.z_axis);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}