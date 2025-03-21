
#include <stdio.h>
#include <xtimer.h>
#include "mma8x5x.h"

#define SLEEP_INTERVAL (5 * 1000 * 1000U)  /* 5 seconds in microseconds */

static mma8x5x_t dev;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .mode = MMA8X5X_MODE_NORMAL,
    .rate = MMA8X5X_RATE_1_56HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    int16_t x, y, z;

    /* Initialize the MMA8x5x sensor */
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x sensor failed");
        return 1;
    }

    while (1) {
        /* Read the accelerometer data */
        if (mma8x5x_read(&dev, &x, &y, &z) == MMA8X5X_OK) {
            printf("Accelerometer data: x=%i y=%i z=%i\n", x, y, z);
        } else {
            puts("Failed to read accelerometer data");
        }

        /* Sleep for 5 seconds */
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
