#include xtimer.h
#include evtimer.h
#include container.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "periph/i2c.h"
#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed
");
        return 1;
    }

    while(1) {
        mma8x5x_data_t data;
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            puts("Error reading data
");
        } else {
            printf("x: %d, y: %d, z: %d\n", data.x, data.y, data.z);
        }
        xtimer_sleep(5);
    }
    return 0;
}