#include "xtimer.h"
#include "thread.h"
#include "mma8x5x.h"
#include "i2c.h"

#define SLEEP_DURATION  (5U * 1000000U) // sleep duration in microseconds

enum { THREAD_STACKSIZE_MAIN = (THREAD_STACKSIZE_DEFAULT) };

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    /* Initialize I2C and MMA8x5x accelerometer */
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization of the accelerometer i2c failed\n");
        return 1;
    }

    while (1) {
        /* Read data from the accelerometer */
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read accelerometer data\n");
        }

        /* Sleep for defined duration */
        xtimer_usleep(SLEEP_DURATION);
    }
    return 0;
}