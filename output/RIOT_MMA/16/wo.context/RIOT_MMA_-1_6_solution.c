#include <stdio.h>
#include "mma8x5x.h"
#include "xtimer.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x!\n");
        return 1;
    }

    while (1) {
        xtimer_sleep(5);
        mma8x5x_read(&dev, &data);
        printf("Acceleration[x]: %d, Acceleration[y]: %d, Acceleration[z]: %d\n", data.x, data.y, data.z);
    }
    return 0;
}