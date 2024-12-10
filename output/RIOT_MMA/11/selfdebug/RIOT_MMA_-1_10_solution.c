#include <xtimer.h>
#include <stdio.h>
#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void read_accelerometer(void) {
    mma8x5x_read(&dev, &data);
    printf("Acceleration X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
}

int main(void) {
    printf("MMA8X5X accelerometer test\n");

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    while (1) {
        read_accelerometer();
        xtimer_sleep(5);  // sleep for 5 seconds
    }

    return 0;
}