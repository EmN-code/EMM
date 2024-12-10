
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS

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

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("MMA8x5x initialization failed\n");
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data: x = %d, y = %d, z = %d\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }

    return 0;
}
