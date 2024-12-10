
#include <stdio.h>
#include "xtimer.h"
#include "mma8x5x.h"

#define MMA8X5X_I2C_DEV   I2C_DEV(0)  // Assuming I2C device 0 is used
#define MMA8X5X_ADDR      0x1D        // MMA8x5x I2C address

static mma8x5x_t mma8x5x;

int main(void) {
    // Initialize the MMA8x5x sensor
    mma8x5x_params_t params = {
        .i2c_dev = MMA8X5X_I2C_DEV,
        .i2c_addr = MMA8X5X_ADDR,
        .mode = MMA8X5X_MODE_NORMAL,
        .range = MMA8X5X_RANGE_2G,
        .rate = MMA8X5X_RATE_1_56HZ
    };

    if (mma8x5x_init(&mma8x5x, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x sensor");
        return 1;
    }

    int16_t x, y, z;
    while (1) {
        // Read acceleration data
        if (mma8x5x_read(&mma8x5x, &x, &y, &z) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", x, y, z);
        } else {
            puts("Failed to read acceleration data");
        }

        // Delay for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
