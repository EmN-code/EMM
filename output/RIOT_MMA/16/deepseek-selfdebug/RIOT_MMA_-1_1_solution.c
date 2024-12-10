
#include <stdio.h>
#include <xtimer.h>
#include <mma8x5x.h>

#define SLEEP_INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static mma8x5x_t dev;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_800HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_mma8x5x(void) {
    int16_t x, y, z;
    if (mma8x5x_read(&dev, &x, &y, &z) == 0) {
        printf("MMA8x5x: x=%i y=%i z=%i\n", x, y, z);
    } else {
        puts("Failed to read from MMA8x5x sensor");
    }
}

int main(void) {
    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(&dev, &params) != 0) {
        puts("Initialization of MMA8x5x sensor failed");
        return 1;
    }

    while (1) {
        read_mma8x5x();
        xtimer_usleep(SLEEP_INTERVAL_MS);
    }

    return 0;
}
