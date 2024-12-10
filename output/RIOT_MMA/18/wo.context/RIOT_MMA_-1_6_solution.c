#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "ztimer.h"

#define I2C_ADDRESS (0x1D)

static mma8x5x_t dev;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    mma8x5x_data_t data;
    printf("MMA8x5x initialized and reading data every 5 seconds\n");

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read data from MMA8x5x\n");
        }
        xtimer_sleep(5); // Wait for 5 seconds
    }
    return 0;
}