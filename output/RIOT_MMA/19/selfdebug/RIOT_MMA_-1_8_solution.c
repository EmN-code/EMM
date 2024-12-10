#include <stdio.h>
#include <stdlib.h>
#include <periph/i2c.h>
#include <xtimer.h>
#include "mma8x5x.h"

#define THREAD_PRIORITY_MAIN 6

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_mma8x5x(void)
{
    mma8x5x_data_t data;
    if (mma8x5x_init(&dev, &params) == MMA8X5X_OK) {
        while (1) {
            if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
                printf("X-axis: %d, Y-axis: %d, Z-axis: %d\n", data.x, data.y, data.z);
            } else {
                printf("Failed to read data from MMA8x5x\n");
            }
            xtimer_sleep(5);
        }
    } else {
        printf("Failed to initialize MMA8x5x\n");
    }
}

int main(void)
{
    printf("Starting MMA8x5x sensor reading\n");
    read_mma8x5x();
    return 0;
}