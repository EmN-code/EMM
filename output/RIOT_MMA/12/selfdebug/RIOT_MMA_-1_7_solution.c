#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include <stdio.h>

static void read_mma8x5x(mma8x5x_t *dev) {
    mma8x5x_data_t data;
    int res = mma8x5x_read(dev, &data);
    if (res == 0) {
        printf("X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
    } else {
        printf("Error reading MMA8x5x sensor\n");
    }
}

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        read_mma8x5x(&dev);
        xtimer_sleep(5);
    }

    return 0;  // This line is never reached
}