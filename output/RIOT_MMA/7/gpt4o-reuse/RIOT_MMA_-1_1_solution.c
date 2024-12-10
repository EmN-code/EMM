
#include <stdio.h>
#include <xtimer.h>
#include <mma8x5x.h>

#define MMA8X5X_I2C_ADDRESS 0x1D
#define MMA8X5X_I2C_DEV I2C_DEV(0)

mma8x5x_t dev;
mma8x5x_params_t params = {
    .i2c = MMA8X5X_I2C_DEV,
    .addr = MMA8X5X_I2C_ADDRESS,
    .mode = MMA8X5X_MODE_NORMAL,
    .rate = MMA8X5X_RATE_100HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x sensor failed");
        return 1;
    }

    int16_t x, y, z;
    while (1) {
        // Read acceleration data
        if (mma8x5x_read(&dev, &x, &y, &z) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", x, y, z);
        } else {
            puts("Failed to read acceleration data");
        }

        // Delay for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
