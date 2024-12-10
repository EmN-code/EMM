#include <thread.h>
#include <time_units.h>
#include <mma8x5x.h>
#include <event/thread.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <stdio.h>

#define I2C_DEV(i)        (i)
#define MMA8X5X_I2C_ADDRESS (0x1D)
#define MMA8X5X_RATE_50HZ   (50)
#define MMA8X5X_RANGE_2G    (2)
#define US_PER_SEC          (1000000)

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c   = I2C_DEV(0),
        .addr  = MMA8X5X_I2C_ADDRESS,
        .rate  = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x: %d\n", res);
        return 1;
    }

    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            int16_t acc_vals[3];
            mma8x5x_read(&dev, &data);
            printf("Acceleration X: %d, Y: %d, Z: %d\n", data.acc.x, data.acc.y, data.acc.z);
        }
        thread_sleep((uint32_t)(5 * US_PER_SEC));
    }

    return 0;
}
