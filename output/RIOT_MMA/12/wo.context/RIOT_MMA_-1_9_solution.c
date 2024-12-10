#include <xtimer.h>
#include "mma8x5x.h"
#include <stdio.h>

int main(void) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed!");
        return -1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Acceleration: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        } else {
            puts("Error reading accelerometer data");
        }
        xtimer_sleep(5);
    }

    return 0;
}