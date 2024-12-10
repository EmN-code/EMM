#include "periph/i2c.h"
#include <stdio.h>
#include "driver_mma8x5x.h"

#define I2C_DEV     0
#define MMA8X5X_ADDR    0x1D
#define READ_INTERVAL   (5LU)

static mma8x5x_t dev;

int main(void) {
    mma8x5x_params_t params;
    params.i2c = I2C_DEV;
    params.addr = MMA8X5X_ADDR;
    params.rate = MMA8X5X_RATE_50HZ;
    params.range = MMA8X5X_RANGE_2G;

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize device\n");
        return -1;
    }

    while (1) {
        mma8x5x_data_t data;
        mma8x5x_read(&dev, &data);
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        xtimer_sleep(READ_INTERVAL);
    }
    return 0;
}