
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time_units.h>
#include <xtimer.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <msg.h>
#include <stdatomic.h>
#include <kernel_defines.h>
#include <timex.h>
#include <stddef.h>
#include <mbox.h>

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS

static mma8x5x_t dev;
mma8x5x_data_t data;

mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    int res;

    printf("Initializing MMA8x5x accelerometer\n");
    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    while (1) {
        xtimer_sleep(5);
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data - X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    }

    return 0;
}
