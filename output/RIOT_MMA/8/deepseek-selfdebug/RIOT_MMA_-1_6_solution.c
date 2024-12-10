
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"

#define I2C_DEV(x) (x)
#define MMA8X5X_I2C_ADDRESS MMA8X5X_ADDR

int main(void)
{
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    while (1) {
        mma8x5x_data_t data;
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Accelerometer data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read accelerometer data\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}
