
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "thread.h"

#define SLEEP_INTERVAL (5 * 1000 * 1000U)  // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_accelerometer(void) {
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Accelerometer data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
    } else {
        puts("Failed to read accelerometer data");
    }
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x sensor failed");
        return 1;
    }

    while (1) {
        read_accelerometer();
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
