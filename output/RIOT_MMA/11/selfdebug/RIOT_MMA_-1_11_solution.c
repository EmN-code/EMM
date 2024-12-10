#include <xtimer.h>
#include <stdio.h>
#include <periph/i2c.h>
#include "mma8x5x.h"

#define DELAY 5U * 1000000U  // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void)
{
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,  // Use the correct I2C address macro
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("MMA8x5x initialization failed");
        return 1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            puts("Failed to read data");
        } else {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        xtimer_sleep(DELAY);
    }

    return 0;
}