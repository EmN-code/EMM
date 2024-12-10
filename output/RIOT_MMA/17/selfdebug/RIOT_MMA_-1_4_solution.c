#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include <stdio.h>

#define I2C_DEV      (I2C_DEV(0))
#define ADDR         (MMA8X5X_I2C_ADDRESS)
#define RATE         (MMA8X5X_RATE_50HZ)
#define RANGE        (MMA8X5X_RANGE_2G)
#define INTERVAL     (5U * 1000000U)  // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = ADDR,
        .rate = RATE,
        .range = RANGE
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer\n");
        return 1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read data\n");
        }
        xtimer_usleep(INTERVAL);  // Delay for 5 seconds
    }

    return 0;
}