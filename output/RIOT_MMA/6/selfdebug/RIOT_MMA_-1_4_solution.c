#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_DEV (I2C_DEV(0))
#define ADDR (0x1D)  // Default I2C address for MMA8x5x
#define RATE (MMA8X5X_RATE_50HZ)
#define RANGE (MMA8X5X_RANGE_2G)
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void)
{
    // Initialize the parameters for MMA8x5x
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = ADDR,
        .rate = RATE,
        .range = RANGE
    };

    // Initialize the accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8x5x_OK) {
        puts("Failed to initialize MMA8x5x");
        return 1;
    }

    while (1) {
        // Read sensor data
        mma8x5x_read(&dev, &data);

        // Print accelerometer data
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x_axis, data.y_axis, data.z_axis);

        // Wait for 5 seconds
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}