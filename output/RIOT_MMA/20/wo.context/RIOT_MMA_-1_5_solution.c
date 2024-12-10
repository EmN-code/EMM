#include "ztimer.h"
#include "mma8x5x.h"
#include "i2c.h"
#include <stdio.h>

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_mma8x5x(void) {
    mma8x5x_data_t data;
    mma8x5x_read(&dev, &data);
    printf("Acceleration [mg]: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    while (1) {
        read_mma8x5x();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}