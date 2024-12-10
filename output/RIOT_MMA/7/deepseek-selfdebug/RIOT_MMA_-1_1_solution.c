
#include <stdio.h>
#include <xtimer.h>
#include <mma8x5x.h>

#define SLEEP_SECONDS 5

static mma8x5x_t dev;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_DEFAULT_ADDRESS,
    .rate = MMA8X5X_RATE_100HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    int16_t x, y, z;

    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x sensor failed");
        return 1;
    }

    while (1) {
        // Read the accelerometer data
        if (mma8x5x_read(&dev, &x, &y, &z) == MMA8X5X_OK) {
            printf("Accelerometer data: x=%d, y=%d, z=%d\n", x, y, z);
        } else {
            puts("Failed to read accelerometer data");
        }

        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_SECONDS);
    }

    return 0;
}
