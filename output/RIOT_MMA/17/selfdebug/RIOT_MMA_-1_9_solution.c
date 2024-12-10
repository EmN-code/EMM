#include <xtimer.h>
#include <stdio.h>
#include <periph/i2c.h>
#include "mma8x5x.h"

#define CONFIG_MMA8X5X_I2C_ADDRESS 0x1D  // Define the I2C address
#define SLEEP_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

int main(void) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8X5X");
        return 1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Acceleration [mg]: X=%d Y=%d Z=%d\n",
                   data.offset[0], data.offset[1], data.offset[2]);
        } else {
            puts("Error reading MMA8X5X data");
        }
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}