
#include <stdio.h>
#include <assert.h>
#include "xtimer.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "event/periodic_callback.h"
#include "event/thread.h"

#define SLEEP_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void read_sensor(void) {
    int res = mma8x5x_read(&dev, &data);
    if (res == MMA8X5X_OK) {
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read accelerometer data\n");
    }
}

static void periodic_callback(void *arg) {
    (void)arg;
    read_sensor();
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    assert(res == MMA8X5X_OK);

    event_periodic_callback_t periodic;
    event_periodic_callback_init(&periodic, xtimer_now(), SLEEP_INTERVAL, periodic_callback, NULL);

    while (1) {
        event_periodic_callback_event_loop(&periodic);
    }

    return 0;
}
