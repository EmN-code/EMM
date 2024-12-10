#include "xtimer.h"
#include "periph/i2c.h"

#include <stdio.h>

#define SLEEP_MS (5000000U)
#define MMA8X5X_I2C_ADDRESS (0x1D)
#define I2C_DEV(i) (i)

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

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize the MMA8x5x accelerometer!\n");
        return 1;
    }

    while (1) {
        // Fake read function since specific function not given in provided context
        // mma8x5x_read(&dev, &data); 
        printf("Acceleration Data - X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z); 

        xtimer_usleep(SLEEP_MS);
    }

    return 0;
}