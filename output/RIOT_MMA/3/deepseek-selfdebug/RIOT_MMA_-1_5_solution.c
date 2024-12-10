
#include <stdio.h>
#include "mma8x5x.h"
#include "periph/i2c.h"
#include "xtimer.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define SLEEP_5_SECONDS (5 * 1000 * 1000U)

int main(void)
{
    mma8x5x_t dev;
    mma8x5x_data_t data;

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int ret = mma8x5x_init(&dev, &params);
    if (ret != MMA8X5X_OK) {
        printf("Initialization failed with error code %d\n", ret);
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration [mg]: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        xtimer_usleep(SLEEP_5_SECONDS);
    }

    return 0;
}
