#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "mma8x5x.h"
#include "periph/i2c.h"

#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_ADDR_0
};

static void read_mma8x5x(void)
{
    int16_t acc[3];
    if (mma8x5x_read(&dev, acc) != 0) {
        printf("Error reading MMA8x5x sensor\n");
        return;
    }
    printf("Acceleration: X=%d mg, Y=%d mg, Z=%d mg\n", acc[0], acc[1], acc[2]);
}

int main(void)
{
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Initialization failed\n");
        return 1;
    }
    printf("MMA8x5x sensor initialized\n");

    while (1) {
        read_mma8x5x();
        xtimer_sleep(INTERVAL / 1000000U);  // convert microseconds to seconds
    }

    return 0;
}