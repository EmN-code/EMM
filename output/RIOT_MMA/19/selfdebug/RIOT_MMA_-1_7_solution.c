#include "ztimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define SLEEP_DURATION (5U * ZTIMER_MSEC_PER_SEC)

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    printf("Initializing MMA8x5x accelerometer...\n");

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x, error code: %d\n", res);
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);

        ztimer_sleep(ZTIMER_MSEC, SLEEP_DURATION);
    }

    return 0;
}