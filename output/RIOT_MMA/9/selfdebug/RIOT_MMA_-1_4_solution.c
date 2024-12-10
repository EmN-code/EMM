#include <periph/i2c.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include "mma8x5x.h"

#define I2C_ADDR       CONFIG_MMA8X5X_I2C_ADDRESS
#define SLEEP_DURATION (5U * 1000000U) // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = I2C_ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the MMA8x5x device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        // Read data from the MMA8x5x sensor
        mma8x5x_read(&dev, &data);
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x_axis, data.y_axis, data.z_axis);

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_DURATION);
    }

    return 0;
}