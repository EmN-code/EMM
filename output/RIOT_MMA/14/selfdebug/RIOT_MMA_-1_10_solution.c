#include <stdio.h>
#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C  I2C_DEV(0)
#define ADDR 0x1D /* Assuming this is the default I2C address for MMA8653 */
#define DELAY  5U  /* delay in seconds */

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C,
    .addr = ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_mma8x5x(void) {
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        printf("Error reading data from MMA8x5x\n");
    }
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        read_mma8x5x();
        xtimer_sleep(DELAY);
    }

    return 0;
}