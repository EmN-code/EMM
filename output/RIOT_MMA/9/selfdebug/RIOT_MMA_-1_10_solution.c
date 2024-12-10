#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    // Initialize MMA8x5x parameters
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }
    puts("MMA8x5x sensor initialized\n");

    // Reading loop
    while (1) {
        // Fetch the sensor values
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            puts("Failed to read data\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}