#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread_config.h"
#include "time_units.h"
#include "thread.h"
#include "stdio.h"
#include "msg.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "dht_params.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_mma8x5x(void *arg) {
    (void)arg;
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        // Adjust to print correct accessed structure members
        printf("Acceleration data: x=%d mg, y=%d mg, z=%d mg\n",
                data.acc[0], data.acc[1], data.acc[2]);
        usleep(5000000); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid;
    if ((pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, read_mma8x5x,
                                NULL, "mma8x5x_reader")) <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    return 0;
}