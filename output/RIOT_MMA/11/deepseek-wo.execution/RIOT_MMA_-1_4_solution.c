
#include <stdio.h>
#include <stdlib.h>
#include <time_units.h>
#include <xtimer.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>

#define MMA8X5X_I2C_ADDRESS 0x1D  // Define the I2C address
#define MMA8X5X_RATE_50HZ 0x06    // Define the rate
#define MMA8X5X_RANGE_2G 0x00     // Define the range

static mma8x5x_t dev;
mma8x5x_data_t data;

int main(void)
{
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int result = mma8x5x_init(&dev, &params);
    if (result != MMA8X5X_OK) {
        printf("Initialization failed with error code %d\n", result);
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
