#include <xtimer.h>
#include <stdio.h>
#include "mma8x5x.h"
#include "periph/i2c.h"
#include "thread.h"

#define READ_INTERVAL (5U * 1000000U) // 5 seconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

void *mma8x5x_thread(void *arg) {
    (void)arg;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed
");
        return NULL;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
        } else {
            puts("Failed to read data
");
        }
        xtimer_usleep(READ_INTERVAL);
    }
    return NULL;
}

int main(void) {
    char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mma8x5x_thread, NULL, "mma8x5x");
    return 0;
}