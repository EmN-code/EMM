#include <stdio.h>
#include <string.h>
#include "periph_cpu.h"
#include "mma8x5x.h"

#define SENSOR_RATE MMA8X5X_RATE_50HZ
#define SENSOR_RANGE MMA8X5X_RANGE_2G

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = SENSOR_RATE,
        .range = SENSOR_RANGE
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x accelerometer");
        return res;
    }

    while (1) {
        mma8x5x_data_t data;
        res = mma8x5x_read(&dev, &data);
        if (res == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.offset[0], data.offset[1], data.offset[2]);
        } else {
            puts("Failed to read data from MMA8x5x");
        }
        xtimer_sleep(5);
    }

    return 0;
}