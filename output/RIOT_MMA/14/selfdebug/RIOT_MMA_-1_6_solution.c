#include <xtimer.h>
#include <stdio.h>
#include "mma8x5x.h"   // Assuming the library for mma8x5x is included in this

#define I2C_DEV(i) (i)
#define MMA8X5X_I2C_ADDRESS 0x1D
#define MMA8X5X_RATE_50HZ 50
#define MMA8X5X_RANGE_2G 2
#define INTERVAL 5U

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != 0) {
        printf("Device initialization failed with error code: %d\n", res);
        return 1;
    }

    while (1) {
        int16_t x, y, z;
        int err = mma8x5x_read_raw(&dev, &x, &y, &z);
        if (err != 0) {
            printf("Failed to read data with error code: %d\n", err);
        } else {
            printf("Accel Data -> X: %d | Y: %d | Z: %d\n", x, y, z);
        }

        xtimer_sleep(INTERVAL);
    }

    return 0;
}