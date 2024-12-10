
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "log.h"

#define MMA8X5X_I2C_ADDRESS (0x1D)  // Correct I2C address for MMA8x5x

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    int res;
    mma8x5x_data_t data;

    // Initialize the MMA8x5x accelerometer
    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        LOG_ERROR("MMA8x5x initialization failed with error code %d\n", res);
        return 1;
    }

    while (1) {
        // Read accelerometer data
        res = mma8x5x_read(&dev, &data);
        if (res == MMA8X5X_OK) {
            LOG_INFO("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        } else {
            LOG_ERROR("Failed to read accelerometer data with error code %d\n", res);
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
