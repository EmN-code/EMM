
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "log.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define MMA8X5X_RATE MMA8X5X_RATE_50HZ
#define MMA8X5X_RANGE MMA8X5X_RANGE_2G

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE,
    .range = MMA8X5X_RANGE
};

int main(void)
{
    int res;
    mma8x5x_data_t data;

    LOG_INFO("Initializing MMA8x5x accelerometer\n");
    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        LOG_ERROR("Failed to initialize MMA8x5x accelerometer (error %d)\n", res);
        return 1;
    }

    while (1) {
        res = mma8x5x_read(&dev, &data);
        if (res == MMA8X5X_OK) {
            LOG_INFO("Accelerometer data - X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            LOG_ERROR("Failed to read accelerometer data (error %d)\n", res);
        }

        xtimer_sleep(5);
    }

    return 0;
}
