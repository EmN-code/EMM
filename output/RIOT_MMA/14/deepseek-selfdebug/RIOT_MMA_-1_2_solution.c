
#include "mma8x5x.h"
#include "periph/i2c.h"
#include "xtimer.h"
#include <stdio.h>

#define MMA8X5X_I2C_ADDRESS (0x1D)  // Correct I2C address for MMA8653

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x sensor failed");
        return 1;
    }

    mma8x5x_data_t data;

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration [mg]: X: %d Y: %d Z: %d\n", data.x, data.y, data.z);
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
