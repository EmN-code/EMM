
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"

#define I2C_DEV_NUM         (0)
#define MMA8X5X_I2C_ADDRESS (0x1D)  // Assuming this is the default I2C address for MMA8653

int main(void)
{
    mma8x5x_t dev;
    mma8x5x_data_t data;

    mma8x5x_params_t params = {
        .i2c = I2C_DEV_NUM,
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
