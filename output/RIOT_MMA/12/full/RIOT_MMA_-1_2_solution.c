
#include "thread.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "thread_flags.h"
#include "host/ble_ibeacon.h"
#include <stdio.h>
#include "timex.h"
#include "xtimer.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static mma8x5x_t dev;

void read_accel_data(void) {
    mma8x5x_data_t data;
    mma8x5x_read(&dev, &data);
    printf("Acceleration [mg]: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
}

void *thread_func(void *arg) {
    (void)arg; // Unused parameter
    while (1) {
        read_accel_data();
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  thread_func, NULL, "accel_thread");

    return 0;
}