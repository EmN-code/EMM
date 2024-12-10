#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "xtimer.h"
#include "ztimer.h"
#include "thread.h"

#define SLEEP_TIME_SEC (5U)

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_accelerometer(void) {
    if (mma8x5x_read(&dev, &data) == 0) {
        printf("Acceleration: x=%dmg, y=%dmg, z=%dmg\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read data from accelerometer\n");
    }
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer\n");
        return 1;
    }

    while (1) {
        read_accelerometer();
        ztimer_sleep(ZTIMER_SEC, SLEEP_TIME_SEC);
    }

    return 0;
}