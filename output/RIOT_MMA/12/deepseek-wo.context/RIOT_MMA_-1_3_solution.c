
#include <stdio.h>  // Include stdio.h to resolve the implicit declaration of printf
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"

#define I2C_DEV         I2C_DEV(0)
#define MMA8X5X_ADDR    0x1D
#define MMA8X5X_RATE    MMA8X5X_RATE_50HZ
#define MMA8X5X_RANGE   MMA8X5X_RANGE_2G
#define SLEEP_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    mma8x5x_t dev;
    mma8x5x_data_t data;

    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = MMA8X5X_ADDR,
        .rate = MMA8X5X_RATE,
        .range = MMA8X5X_RANGE
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("MMA8x5x accelerometer initialization failed\n");
        return 1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Accelerometer data: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read accelerometer data\n");
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
