
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "thread.h"

#define I2C_DEV         I2C_DEV(0)
#define MMA8X5X_ADDR    MMA8X5X_I2C_ADDRESS
#define SLEEP_SECONDS   5

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void read_accelerometer(void) {
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
    } else {
        puts("Failed to read accelerometer data");
    }
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = MMA8X5X_ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x accelerometer");
        return 1;
    }

    while (1) {
        read_accelerometer();
        xtimer_sleep(SLEEP_SECONDS);
    }

    return 0;
}
