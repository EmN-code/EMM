#include "xtimer.h"
#include "stdio.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "periph/i2c.h"
#include "msg.h"
#include "unistd.h"
#include "thread.h"

#define SLEEP_DURATION       (5U * US_PER_SEC)

static char mma8x5x_thread_stack[THREAD_STACKSIZE_MAIN];

void *mma8x5x_thread_handler(void *arg) {
    (void)arg;
    mma8x5x_data_t acc_data;
    while (1) {
        mma8x5x_read(&dev, &acc_data);
        printf("Acceleration: X=%d mg, Y=%d mg, Z=%d mg\n",
               acc_data.x, acc_data.y, acc_data.z);
        xtimer_sleep(SLEEP_DURATION);
    }
    return NULL;
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    kernel_pid_t mma8x5x_thread = thread_create(mma8x5x_thread_stack, sizeof(mma8x5x_thread_stack),
                                                 THREAD_PRIORITY_MAIN - 1, 0,
                                                 mma8x5x_thread_handler, NULL, "mma8x5x_thread");
    if (mma8x5x_thread < 0) {
        printf("Failed to create MMA8x5x thread\n");
        return 1;
    }

    return 0;
}