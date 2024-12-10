#include <xtimer.h>
#include <periph/i2c.h>
#include "mma8x5x.h"

#define DELAY 5

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,  // Correct constant
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed.\n");
        return 1;
    }

    while (1) {
        int res = mma8x5x_read(&dev, &data);
        if (res == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read data.\n");
        }
        xtimer_sleep(DELAY);
    }
    return 0;
}