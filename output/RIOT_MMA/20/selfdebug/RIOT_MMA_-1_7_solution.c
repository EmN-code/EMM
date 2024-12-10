#include "xtimer.h"
#include "mma8x5x.h"
#include <stdio.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x sensor");
        return 1;
    }

    while (1) {
        // Read data from the sensor
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            puts("Failed to read data from MMA8x5x sensor");
        } else {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        
        // Wait 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
