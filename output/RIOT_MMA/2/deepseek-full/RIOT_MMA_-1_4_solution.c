
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "thread.h"
#include "stdio.h"

#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void accelerometer_thread(void *arg)
{
    (void)arg;

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        xtimer_usleep(SLEEP_INTERVAL);
    }
}

int main(void)
{
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        accelerometer_thread,
        NULL,
        "accelerometer_thread"
    );

    return 0;
}
