#include <xtimer.h>
#include <stdio.h>
#include "mma8x5x.h"
#include "periph/i2c.h"

#define I2C_ADDR         MMA8X5X_I2C_ADDRESS
#define INTERVAL         5000000   // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = I2C_ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    printf("Initializing MMA8x5x sensor...\n");
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    printf("Successfully initialized MMA8x5x sensor\n");

    while (1) {
        mma8x5x_data_t data;
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            printf("Failed to read data from sensor\n");
        } else {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        xtimer_usleep(INTERVAL);
    }

    return 0;
}