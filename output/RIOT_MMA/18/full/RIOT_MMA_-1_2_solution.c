#include <stdio.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <event/thread.h>
#include <thread.h>

#define SLEEP_INTERVAL (5U)

static mma8x5x_t dev;
static mma8x5x_data_t data;

int read_mma8x5x(void) {
    int result = mma8x5x_read(&dev, &data);
    if (result == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
    } else {
        printf("Failed to read MMA8x5x data\n");
    }
    return result;
}

int main(void) {
    printf("MMA8x5x Accelerometer Example\n");

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    while (1) {
        read_mma8x5x();
        thread_sleep(SLEEP_INTERVAL);
    }

    return 0;
}