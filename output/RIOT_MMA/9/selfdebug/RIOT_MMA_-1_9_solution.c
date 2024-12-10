#include "stdlib.h"
#include "stdio.h"
#include "mma8x5x.h" // Make sure to include your accelerometer driver's header file
#include "kernel_defines.h" // When using functions from the RIOT kernel
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "i2c.h"

#define SLEEP_INTERVAL (5U * US_PER_SEC) // Sleep for 5 seconds

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];

void *mma8x5x_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    mma8x5x_t dev;
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer\n");
        return NULL;
    }

    while (1) {
        mma8x5x_data_t data;
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
        } else {
            printf("Read error\n");
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACK, mma8x5x_thread, NULL, "mma8x5x");
    return 0;
}
