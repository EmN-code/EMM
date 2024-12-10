
#include <stdio.h>
#include <xtimer.h>
#include "mma8x5x.h"

#define SLEEP_5_SECONDS   (5 * 1000 * 1000U)

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void)
{
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
        res = mma8x5x_read(&dev, &data);
        if (res == MMA8X5X_OK) {
            printf("Accelerometer data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read accelerometer data with error code %d\n", res);
        }

        xtimer_usleep(SLEEP_5_SECONDS);
    }

    return 0;
}
