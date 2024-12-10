
#include <stdio.h>
#include <xtimer.h>
#include "mma8x5x.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define MMA8X5X_RATE MMA8X5X_RATE_50HZ

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    int res;
    mma8x5x_data_t data;

    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }

    return 0;
}
