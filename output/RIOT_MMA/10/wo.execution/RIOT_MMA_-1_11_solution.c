#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"

#include "mma8x5x.h"

#define SLEEP_INTERVAL 5

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
    printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error initializing accelerometer\n");
        return 1;
    }

    while (1) {
        read_mma8x5x();
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}