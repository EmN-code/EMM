#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = 0x1D, // Assume the default address
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed!");
        return 1;
    }

    while (1) {
        mma8x5x_data_t data;
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.offset[0], data.offset[1], data.offset[2]);
        } else {
            puts("Error reading data!");
        }
        xtimer_usleep(INTERVAL);
    }
    return 0;
}