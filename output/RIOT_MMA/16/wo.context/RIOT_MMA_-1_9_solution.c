#include <stdio.h>
#include <thread.h>
#include <timex.h>
#include <xtimer.h>
#include <periph/i2c.h>
#include "mma8x5x.h"

#define I2C_ADDR        (0x1D)
#define SLEEP_INTERVAL  (5U * US_PER_SEC)

static kernel_pid_t thread_pid;
static char thread_stack[THREAD_STACKSIZE_MAIN];

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = I2C_ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};
static mma8x5x_data_t data;

void *mma8x5x_read_thread(void *arg) {
    (void)arg;
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, SLEEP_INTERVAL);
        mma8x5x_read(&dev, &data);
        printf("Acceleration: x = %d, y = %d, z = %d\n", data.x, data.y, data.z);
    }
    return NULL;
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize mma8x5x\n");
        return 1;
    }
    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                               mma8x5x_read_thread, NULL, "mma8x5x_read");
    return 0;
}
