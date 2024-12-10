#include "xtimer.h"
#include "msg.h"
#include "stdio.h"
#include "mma8x5x.h"

/**
 * This simple program demonstrates to read the MMA8x5x accelerometer sensor every 5 seconds.
 */

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void)
{
    puts("MMA8x5x accelerometer test!");

    /* default parameters */
    mma8x5x_params_t params;
    params.i2c = I2C_DEV(0);
    params.addr = CONFIG_MMA8X5X_I2C_ADDRESS; // Ensure this constant is declared elsewhere
    params.rate = MMA8X5X_RATE_50HZ;
    params.range = MMA8X5X_RANGE_2G;

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed!");
        return 1;
    }

    while (1) {
        xtimer_sleep(5); // Sleep for 5 seconds

        mma8x5x_read(&dev, &data);

        printf("Acceleration: x=%d mg, y=%d mg, z=%d mg\n",
               data.x_axis, data.y_axis, data.z_axis);
    }

    return 0;
}
