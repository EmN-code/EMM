#include <xtimer.h>
#include <periph/i2c.h>
#include "mma8x5x.h"

#define I2C_DEV 0
#define MMA8X5X_I2C_ADDRESS 0x1D // Assumed correct I2C address

int main(void) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed");
        return 1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Acceleration X: %d, Y: %d, Z: %d\n", data.values[0], data.values[1], data.values[2]);
        } else {
            puts("Read failed");
        }
        xtimer_sleep(5);
    }
    return 0;
}