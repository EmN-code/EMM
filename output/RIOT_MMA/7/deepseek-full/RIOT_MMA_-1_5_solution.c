
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ztimer.h"
#include "msg.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define STACKSIZE 512
#define PRIORITY 5
#define MSG_QUEUE_SIZE 8

static mma8x5x_t dev;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void accelerometer_thread(void *arg)
{
    (void)arg;
    mma8x5x_data_t data;

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x sensor failed");
        return 1;
    }

    kernel_pid_t pid = thread_create(NULL, STACKSIZE, PRIORITY, 0, accelerometer_thread, NULL, "accelerometer_thread");
    if (pid <= KERNEL_PID_UNDEF) {
        puts("Failed to create accelerometer thread");
        return 1;
    }

    return 0;
}
