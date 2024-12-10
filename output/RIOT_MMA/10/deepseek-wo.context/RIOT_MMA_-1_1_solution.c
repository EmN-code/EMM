
#include <stdio.h>
#include <xtimer.h>
#include <mma8x5x.h>

#define SLEEP_INTERVAL_MS (5 * 1000U)  // 5 seconds

static mma8x5x_t dev;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_1_56HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_mma8x5x(void) {
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == 0) {
        printf("Accelerometer data: x=%.2f, y=%.2f, z=%.2f\n", data.x, data.y, data.z);
    } else {
        puts("Failed to read accelerometer data");
    }
}

int main(void) {
    puts("MMA8x5x accelerometer reader");

    if (mma8x5x_init(&dev, &params) != 0) {
        puts("Initialization of MMA8x5x sensor failed");
        return 1;
    }

    while (1) {
        read_mma8x5x();
        xtimer_sleep(SLEEP_INTERVAL_MS / MS_IN_USEC);
    }

    return 0;
}
