#include "xtimer.h"
#include "mma8x5x.h"
#include "i2c.h"
#include <stdio.h>

static mma8x5x_t dev;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = 0x1d, // Default I2C address for MMA8x5x
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_mma8x5x(void) {
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) != 0) {
        puts("Failed to read from MMA8x5x");
    } else {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    }
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x failed.\n");
        return 1;
    }

    while (1) {
        read_mma8x5x();
        xtimer_sleep(5);
    }

    return 0;
}
