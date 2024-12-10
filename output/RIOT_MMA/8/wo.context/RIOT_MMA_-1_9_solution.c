#include "mma8x5x.h"
#include "periph/i2c.h"
#include "stdio.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void mma8x5x_read_acceleration(void) {
    if (mma8x5x_init(&dev, &params) == MMA8X5X_OK) {
        mma8x5x_read(&dev, &data);
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
    } else {
        printf("Error initializing MMA8x5x sensor\n");
    }
}

int main(void) {
    while (1) {
        mma8x5x_read_acceleration();
        xtimer_sleep(5);
    }
    return 0;
}