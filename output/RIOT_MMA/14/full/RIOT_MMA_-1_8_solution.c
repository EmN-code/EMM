#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "xtimer.h"
#include "stdio.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_mma8x5x(void) {
    mma8x5x_read(&dev, &data);
    printf("Acceleration: x=%dmg y=%dmg z=%dmg\n", data.x, data.y, data.z);
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Accelerometer init failed");
        return 1;
    }

    while (1) {
        read_mma8x5x();
        xtimer_sleep(5);
    }

    return 0;
}