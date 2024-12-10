#include "periph_conf.h"
#include "periph/i2c.h"
#include "timex.h"
#include "ztimer.h"
#include <stdio.h>

#define I2C_ADDR        (0x1D) /* MMA8x5x default I2C address */
#define I2C_DEV         (I2C_DEV(0))
#define TIMER_INTERVAL  (5U * US_PER_SEC) /* Interval of 5 seconds */

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV,
    .addr = I2C_ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

static void timer_callback(void *arg) {
    (void)arg;
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == 0) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    }
    else {
        printf("Failed to read data\n");
    }
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Initialization failed\n");
        return 1;
    }
    ztimer_t timer;
    timer.callback = timer_callback;
    ztimer_set(&timer, TIMER_INTERVAL);
    return 0;
}