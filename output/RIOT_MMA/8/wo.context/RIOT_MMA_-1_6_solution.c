#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "periph/i2c.h"
#include "xtimer.h"
#include "mma8x5x.h"

#define MMA8X5X_I2C_ADDRESS (0x1D)
#define I2C_BUS (I2C_DEV(0))

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_BUS,
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

static void read_mma8x5x(void) {
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == 0) {
        printf("Acceleration: X=%dmg Y=%dmg Z=%dmg\n", data.x, data.y, data.z);
    }
    else {
        puts("Error reading data!");
    }
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("MMA8x5x initialization failed");
        return 1;
    }
    puts("MMA8x5x initialization successful");

    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        read_mma8x5x();
        xtimer_periodic_wakeup(&last_wakeup, 5 * US_PER_SEC);
    }
    return 0;
}