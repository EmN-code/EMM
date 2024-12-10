#include <thread.h>
#include <xtimer.h>
#include <stdio.h>
#include <mma8x5x.h>

#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void read_mma8x5x(void) {
    if (mma8x5x_read(&dev, &data) == 0) {
        printf("Accelerometer data - X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        puts("Error reading accelerometer data.");
    }
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x failed");
        return -1;
    }

    while (1) {
        read_mma8x5x();
        xtimer_sleep(INTERVAL);
    }

    return 0;
}