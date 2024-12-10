#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"

#define I2C_DEV0      I2C_DEV(0)
#define ADDR          MMA8X5X_I2C_ADDRESS
#define RATE          MMA8X5X_RATE_50HZ
#define RANGE         MMA8X5X_RANGE_2G
#define DELAY         5 * US_PER_SEC

static mma8x5x_t dev;
mma8x5x_data_t data;

int main(void)
{
    mma8x5x_params_t params = {
        .i2c = I2C_DEV0,
        .addr = ADDR,
        .rate = RATE,
        .range = RANGE
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
        return 1;
    }

    puts("MMA8x5x initialized successfully");

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Acceleration: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        }
        else {
            puts("Error reading data from MMA8x5x");
        }
        xtimer_sleep(DELAY);
    }

    return 0;
}