#include <xtimer.h>
#include <stdio.h>
#include <periph/i2c.h>
#include <mma8x5x.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    printf("MMA8x5x accelerometer test application\n");

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
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("x: %d, y: %d, z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read data\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}