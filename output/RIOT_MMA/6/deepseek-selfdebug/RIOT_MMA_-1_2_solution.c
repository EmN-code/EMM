
#include "mma8x5x.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "log.h"

#define SLEEP_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

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

    // Initialize the I2C bus
    i2c_init(I2C_DEV(0));

    // Initialize the MMA8x5x accelerometer
    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        LOG_ERROR("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    while (1) {
        // Read accelerometer data
        mma8x5x_read(&dev, &data);

        // Print the accelerometer data
        LOG_INFO("Accelerometer data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
