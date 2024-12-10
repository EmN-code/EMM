#include "xtimer.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_params.h"

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_mma8x5x(void) {
    mma8x5x_data_t data;
    int res = mma8x5x_read(&dev, &data);
    if (res == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        printf("Error reading MMA8x5x\n");
    }
}

int main(void) {
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x\n");
        return 1;
    }

    while (1) {
        read_mma8x5x();
        xtimer_sleep(5);
    }
    return 0;
}