#include <stdio.h>
#include "ztimer.h"
#include "mma8x5x.h"
#include "periph/i2c.h"

#define I2C_DEV         I2C_DEV(0)
#define I2C_ADDR        MMA8X5X_DEFAULT_ADDR
#define TIMER_INTERVAL  (5U * US_PER_SEC) // 5 seconds

static mma8x5x_t dev;
static ztimer_t timer;

static void read_sensor(void)
{
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Acceleration [mg]: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read data from MMA8x5x sensor\n");
    }
}

static void timer_callback(void *arg)
{
    (void)arg;
    read_sensor();
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL);
}

int main(void)
{
    // Initialize I2C
    if (i2c_init(I2C_DEV) != 0) {
        printf("Failed to initialize I2C\n");
        return 1;
    }

    // Initialize MMA8x5x sensor
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = I2C_ADDR,
        .range = MMA8X5X_RANGE_2G,
        .rate = MMA8X5X_RATE_1HZ,
        .offset = {0, 0, 0}
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    // Set up periodic timer
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL);
    timer.callback = timer_callback;

    // Start the timer
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL);

    // Main loop
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }

    return 0;
}