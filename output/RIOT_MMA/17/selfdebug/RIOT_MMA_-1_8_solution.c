#include <xtimer.h>
#include <stdio.h>
#include <periph/i2c.h>
#include <mma8x5x.h>

#define SLEEP_DURATION (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    puts("MMA8x5x accelerometer test application\n");

    static mma8x5x_t dev;
    static mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed\n");
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("x: %d mg\ty: %d mg\tz: %d mg\n", data.x_axis, data.y_axis, data.z_axis);

        xtimer_sleep(SLEEP_DURATION);
    }

    return 0;
}