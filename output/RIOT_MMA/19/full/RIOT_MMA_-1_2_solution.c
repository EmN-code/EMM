#include "thread_config.h"
#include "thread.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include <stdio.h>
#include <unistd.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error: Could not initialize the accelerometer\n");
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X=%d mg, Y=%d mg, Z=%d mg\n", data.x, data.y, data.z);
        usleep(5000000); // Sleep for 5 seconds
    }

    return 0;
}