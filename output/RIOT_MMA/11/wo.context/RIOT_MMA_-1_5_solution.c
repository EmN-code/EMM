#include "periph/i2c.h"
#include "mma8x5x.h"
#include "thread.h"
#include "xtimer.h"

#define I2C_ADDR       (0x1D)      // Default I2C address for MMA8x5x
#define SLEEP_DURATION (5U * US_PER_SEC)    // 5 seconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = I2C_ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_mma8x5x(void) {
    mma8x5x_data_t data;
    if (mma8x5x_init(&dev, &params) == MMA8X5X_OK) {
        while (1) {
            mma8x5x_read(&dev, &data);
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
            xtimer_sleep(SLEEP_DURATION);
        }
    }
    else {
        printf("Failed to initialize MMA8x5x\n");
    }
}

int main(void) {
    read_mma8x5x();
    return 0;
}