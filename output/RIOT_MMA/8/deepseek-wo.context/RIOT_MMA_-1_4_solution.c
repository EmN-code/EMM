
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"

#define MMA8X5X_I2C_ADDRESS 0x1D  // Assuming this is the default I2C address for MMA8653

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

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Accelerometer data - X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read accelerometer data\n");
        }

        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}