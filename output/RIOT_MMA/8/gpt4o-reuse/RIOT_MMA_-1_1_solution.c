
#include <stdio.h>
#include "xtimer.h"
#include "mma8x5x.h"
#include "periph/i2c.h"

#define MMA8X5X_I2C_DEV         I2C_DEV(0)
#define MMA8X5X_I2C_ADDR        0x1D
#define MMA8X5X_SAMPLE_RATE     MMA8X5X_RATE_1_56HZ
#define MMA8X5X_RANGE           MMA8X5X_RANGE_2G

static mma8x5x_t mma8x5x_dev;

void init_mma8x5x(void) {
    mma8x5x_params_t params = {
        .i2c = MMA8X5X_I2C_DEV,
        .addr = MMA8X5X_I2C_ADDR,
        .rate = MMA8X5X_SAMPLE_RATE,
        .range = MMA8X5X_RANGE
    };

    if (mma8x5x_init(&mma8x5x_dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x sensor failed");
        return;
    }

    puts("MMA8x5x sensor initialized");
}

void read_acceleration(void) {
    int16_t x, y, z;

    if (mma8x5x_read(&mma8x5x_dev, &x, &y, &z) != MMA8X5X_OK) {
        puts("Failed to read acceleration data");
        return;
    }

    printf("X: %d, Y: %d, Z: %d\n", x, y, z);
}

int main(void) {
    puts("Starting MMA8x5x sensor application");

    init_mma8x5x();

    while (1) {
        read_acceleration();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
