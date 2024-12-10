
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <xtimer.h>

#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

int main(void)
{
    mma8x5x_t dev;
    mma8x5x_data_t data;

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Initialization failed with error code %d\n", res);
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
