
#include "mma8x5x.h"
#include "periph/i2c.h"
#include "xtimer.h"
#include <stdio.h>

#define MMA8X5X_I2C_ADDRESS 0x1D
#define MMA8X5X_RATE_50HZ   MMA8X5X_RATE_50HZ
#define MMA8X5X_RANGE_2G    MMA8X5X_RANGE_2G

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    int res;
    mma8x5x_data_t data;

    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    while (1) {
        res = mma8x5x_read(&dev, &data);
        if (res == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read data from MMA8x5x sensor\n");
        }

        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
