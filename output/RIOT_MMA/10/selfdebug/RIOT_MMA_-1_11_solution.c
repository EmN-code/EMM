#include <xtimer.h>
#include <stdio.h>
#include "periph/i2c.h"
#include "mma8x5x.h"

#define ADDR_MMA8X5X          0x1D  // Default I2C address for MMA8x5x
#define I2C_BUS               I2C_DEV(0)  // I2C bus
#define SLEEP_INTERVAL        5  // Sleep interval of 5 seconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_BUS,
        .addr = ADDR_MMA8X5X,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x: %d\n", res);
        return -1;
    }

    while (1) {
        res = mma8x5x_read(&dev, &data);
        if (res == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Error reading MMA8x5x: %d\n", res);
        }

        xtimer_sleep(SLEEP_INTERVAL);  // Wait for 5 seconds
    }
    return 0;
}