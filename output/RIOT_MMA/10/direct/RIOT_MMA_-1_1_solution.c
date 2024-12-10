#include <stdio.h>
#include <timex.h>
#include <xtimer.h>
#include "mma8x5x.h"
#include "periph/i2c.h"

#define I2C_DEV  I2C_DEV(0)
#define ADDR     MMA8X5X_DEFAULT_ADDR

int main(void) {
    i2c_init(I2C_DEV);
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = ADDR,
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        puts("Failed to initialize MMA8x5x sensor");
        return 1;
    }

    while (1) {
        int16_t x, y, z;
        if (mma8x5x_read(&dev, &x, &y, &z) != 0) {
            puts("Failed to read data from MMA8x5x sensor");
            return 1;
        }

        printf("Accel X: %d mg, Y: %d mg, Z: %d mg\n", x, y, z);

        xtimer_sleep(5);
    }

    return 0;
}