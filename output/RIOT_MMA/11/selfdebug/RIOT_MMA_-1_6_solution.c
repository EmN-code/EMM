#include <xtimer.h>
#include <periph/i2c.h>
#include <stdio.h>

#include "mma8x5x_params.h"
#include "mma8x5x.h"

#define I2C_ADDR          (0x1D) // Define the correct I2C address here
#define TIMER_INTERVAL    (5U * US_PER_SEC) // 5 seconds

static mma8x5x_t dev;

int main(void) {
    mma8x5x_params_t params;
    params.i2c = I2C_DEV(0);
    params.addr = I2C_ADDR;
    params.rate = MMA8X5X_RATE_50HZ;
    params.range = MMA8X5X_RANGE_2G;

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed!\n");
        return -1;
    }

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        int16_t x, y, z;
        if (mma8x5x_read_accel(&dev, &x, &y, &z) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", x, y, z);
        } else {
            printf("Error reading device!\n");
        }
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
    }

    return 0;
}