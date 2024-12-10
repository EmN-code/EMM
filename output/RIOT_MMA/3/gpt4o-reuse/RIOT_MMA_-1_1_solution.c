
#include <stdio.h>
#include "xtimer.h"
#include "mma8x5x.h"

#define MMA8X5X_I2C_ADDRESS (0x1D)
#define MMA8X5X_I2C_DEV     (I2C_DEV(0))

static mma8x5x_t mma8x5x_dev;

int main(void) {
    // Initialize the MMA8x5x accelerometer
    mma8x5x_params_t params = {
        .i2c = MMA8X5X_I2C_DEV,
        .addr = MMA8X5X_I2C_ADDRESS,
        .mode = MMA8X5X_MODE_NORMAL,
        .sample_rate = MMA8X5X_RATE_1_56HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&mma8x5x_dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x accelerometer");
        return 1;
    }

    int16_t x, y, z;
    while (1) {
        // Read the acceleration data
        if (mma8x5x_read(&mma8x5x_dev, &x, &y, &z) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", x, y, z);
        } else {
            puts("Failed to read acceleration data");
        }

        // Delay for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
