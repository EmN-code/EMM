
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time_units.h>
#include <thread.h>
#include <stddef.h>
#include <mbox.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <xtimer.h>

#define THREAD_STACKSIZE 512
#define MSG_QUEUE_SIZE 16

static char thread_stack[THREAD_STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

void accelerometer_thread(void *arg)
{
    (void)arg;
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("MMA8x5x initialization failed");
        return;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_t *thread = thread_create(thread_stack, sizeof(thread_stack),
                                     THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST,
                                     accelerometer_thread, NULL, "accelerometer");

    if (thread == NULL) {
        puts("Failed to create accelerometer thread");
        return 1;
    }

    return 0;
}
