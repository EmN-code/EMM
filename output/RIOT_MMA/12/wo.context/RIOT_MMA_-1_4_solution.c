#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.hpp"

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_mma8x5x(void)
{
    if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
        puts("Error reading from MMA8x5x");
        return;
    }
    printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
}

int main(void)
{
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Error initializing MMA8x5x");
        return 1;
    }

    while (1) {
        read_mma8x5x();
        xtimer_sleep(5);
    }
    return 0;
}