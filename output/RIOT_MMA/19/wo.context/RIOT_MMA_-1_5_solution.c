#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define SLEEP_DURATION (5U * US_PER_SEC)

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void read_mma8x5x(void) {
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    }
    else {
        printf("Failed to read from the MMA8x5x sensor\n");
    }
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("MMA8x5x initialization failed\n");
        return 1;
    }

    while (1) {
        read_mma8x5x();
        xtimer_sleep(SLEEP_DURATION);
    }

    return 0;
}