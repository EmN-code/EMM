
#include <stdio.h>
#include <xtimer.h>
#include "mma8x5x.h"
#include "periph/i2c.h"

#define I2C_DEV         I2C_DEV(0)
#define MMA8X5X_ADDR    0x1D
#define MMA8X5X_RATE    MMA8X5X_RATE_50HZ
#define MMA8X5X_RANGE   MMA8X5X_RANGE_2G

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV,
    .addr = MMA8X5X_ADDR,
    .rate = MMA8X5X_RATE,
    .range = MMA8X5X_RANGE
};

void read_accelerometer(void) {
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Accelerometer data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read accelerometer data\n");
    }
}

int main(void) {
    printf("Starting MMA8x5x accelerometer example\n");

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    while (1) {
        read_accelerometer();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
