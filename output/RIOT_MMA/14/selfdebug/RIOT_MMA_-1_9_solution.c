#include <xtimer.h>
#include <sched.h>
#include <periph/i2c.h>
#include <mma8x5x.h>
#include <stdio.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error: Unable to initialize accelerometer\n");
        return 1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Acceleration: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Error: Unable to read data\n");
        }
        xtimer_sleep(5);
    }
    return 0;
}