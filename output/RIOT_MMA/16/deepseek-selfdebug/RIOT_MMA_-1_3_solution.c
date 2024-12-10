
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "thread.h"
#include "stdio.h"

#define I2C_DEV         I2C_DEV(0)
#define MMA8X5X_ADDR    MMA8X5X_I2C_ADDRESS
#define SLEEP_INTERVAL  (5 * 1000 * 1000U)  // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV,
    .addr = MMA8X5X_ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    int res;
    mma8x5x_data_t data;

    // Initialize the I2C bus
    i2c_init(I2C_DEV);

    // Initialize the MMA8x5x accelerometer
    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer: %d\n", res);
        return 1;
    }

    while (1) {
        // Read accelerometer data
        mma8x5x_read(&dev, &data);

        // Print the accelerometer data
        printf("Accelerometer data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
