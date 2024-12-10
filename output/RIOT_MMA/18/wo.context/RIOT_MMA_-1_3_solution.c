#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <periph/i2c.h>

#include "mma8x5x.h"

#define I2C_DEV (I2C_DEV(0))  // Correcting I2C_DEV definition
#define SLEEP_INTERVAL (5 * US_PER_SEC)  // 5 seconds interval

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV,
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x: %d,\n", res);
        return 1;
    }

    while (1) {
        mma8x5x_data_t data;
        res = mma8x5x_read(&dev, &data);
        if (res != MMA8X5X_OK) {
            printf("Error reading data from MMA8x5x: %d,\n", res);
        } else {
            printf("X-axis: %d, Y-axis: %d, Z-axis: %d\n", data.off[0], data.off[1], data.off[2]);
        }
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);  // Pause for a defined interval
    }
    return 0;
}