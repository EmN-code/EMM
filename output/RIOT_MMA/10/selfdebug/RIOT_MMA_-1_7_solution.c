#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_DEV         (I2C_DEV(0))
#define ADDR            CONFIG_MMA8X5X_I2C_ADDRESS
#define POLL_INTERVAL   (5U) /* 5 seconds */

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV,
    .addr = ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    int res = mma8x5x_init(&dev, &params);

    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x: %d\n", res);
        return 1;
    }

    printf("MMA8x5x initialized successfully\n");

    while (1) {
        int16_t x, y, z;

        mma8x5x_read(&dev, &x, &y, &z);
        /* Print the accelerometer data */
        printf("x: %d\ty: %d\tz: %d\n", x, y, z);

        /* Sleep for 5 seconds */
        xtimer_sleep(POLL_INTERVAL);
    }

    return 0;
}