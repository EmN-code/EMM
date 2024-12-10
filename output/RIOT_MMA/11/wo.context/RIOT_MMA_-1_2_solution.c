#include "ztimer.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include <stdio.h>

#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

static mma8x5x_data_t data;

int main(void)
{
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x!");
        return 1;
    }
    puts("MMA8x5x initialized successfully");

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration [X]: %d, [Y]: %d, [Z]: %d\n",
               data.x_axis, data.y_axis, data.z_axis);
        xtimer_usleep(INTERVAL);
    }
    return 0;
}