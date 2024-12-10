#include <stdio.h>
#include <stdlib.h>
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"

#define I2C_ADDRESS 0x1D // Assuming this is the correct I2C address for MMA8653
#define I2C_FREQ    (I2C_SPEED_NORMAL)
#define SLEEP_TIME  (5U * US_PER_SEC) // Sleep time of 5 seconds

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    int res;
    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return -1;
    }
    while (1) {
        res = mma8x5x_read(&dev, &data);
        if (res != MMA8X5X_OK) {
            printf("Failed to read from sensor\n");
            continue;
        }
        printf("Acceleration Data: X=%.2f Y=%.2f Z=%.2f\n",
                data.x, data.y, data.z);
        xtimer_sleep(SLEEP_TIME);
    }
    return 0;
}