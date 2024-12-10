#include <stdio.h>
#include "xtimer.h"
#include "mma8x5x.h"

#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    mma8x5x_t dev;
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

    mma8x5x_data_t data;

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x_axis, data.y_axis, data.z_axis);
        } else {
            printf("Failed to read data\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}
