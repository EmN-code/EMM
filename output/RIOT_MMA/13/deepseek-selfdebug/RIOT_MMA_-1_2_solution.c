
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "log.h"

#define MMA8X5X_I2C_ADDRESS (0x1D)  // Correct I2C address for MMA8653

static mma8x5x_t dev;
static mma8x5x_params_t params;

int main(void)
{
    // Initialize I2C
    i2c_init(I2C_DEV(0));

    // Set up MMA8x5x parameters
    params.i2c = I2C_DEV(0);
    params.addr = MMA8X5X_I2C_ADDRESS;
    params.rate = MMA8X5X_RATE_50HZ;
    params.range = MMA8X5X_RANGE_2G;

    // Initialize MMA8x5x device
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        LOG_ERROR("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    mma8x5x_data_t data;

    while (1) {
        // Read accelerometer data
        mma8x5x_read(&dev, &data);

        // Print the accelerometer data
        LOG_INFO("Accelerometer data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}