
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "log.h"

#define I2C_DEV_NUM         (0)
#define MMA8X5X_I2C_ADDRESS (0x1D)
#define SLEEP_INTERVAL      (5 * 1000 * 1000U)  // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV_NUM,
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    int res;
    mma8x5x_data_t data;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize the I2C bus
    i2c_init(I2C_DEV(I2C_DEV_NUM));

    // Initialize the MMA8x5x accelerometer
    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        LOG_ERROR("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    while (1) {
        // Read accelerometer data
        mma8x5x_read(&dev, &data);

        // Log the accelerometer data
        LOG_INFO("Accelerometer data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);

        // Sleep for 5 seconds
        xtimer_periodic_wakeup(&last_wakeup, SLEEP_INTERVAL);
    }

    return 0;
}
