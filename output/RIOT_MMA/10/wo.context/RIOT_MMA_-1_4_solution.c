#include <xtimer.h>
#include <periph/i2c.h>
#include "mma8x5x.h"

int main(void) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    i2c_t i2c = I2C_DEV(0);
    mma8x5x_params_t params = {
        .i2c = i2c,
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read data from MMA8x5x\n");
        }
        xtimer_sleep(5);
    }
    return 0;
}