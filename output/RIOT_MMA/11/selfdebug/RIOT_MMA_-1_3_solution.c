#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define MMA8X5X_ADDR   (0x1D)  // Default I2C address for MMA8653
#define I2C_DEV        (I2C_DEV(0))
#define DELAY_SEC      (5U)

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV,
    .addr = MMA8X5X_ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        puts("Initialization failed");
        return 1;
    }

    while (1) {
        int16_t x = 0, y = 0, z = 0;
        res = mma8x5x_read(&dev, &x, &y, &z);
        if (res != MMA8X5X_OK) {
            puts("Read failed");
        } else {
            printf("Accel data x: %d, y: %d, z: %d\n", x, y, z);
        }
        xtimer_sleep(DELAY_SEC);
    }
    return 0;
}