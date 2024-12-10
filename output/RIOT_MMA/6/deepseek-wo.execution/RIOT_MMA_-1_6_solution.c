
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time_units.h>
#include <xtimer.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <thread.h>
#include <msg.h>

#define STACK_SIZE 512
#define PRIORITY THREAD_PRIORITY_MAIN
#define MSG_QUEUE_SIZE 8

static mma8x5x_t dev;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void accelerometer_thread(void *arg)
{
    (void)arg;
    mma8x5x_data_t data;

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data: x: %d, y: %d, z: %d\n", data.x, data.y, data.z);
        xtimer_sleep(5);
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
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    char stack[STACK_SIZE];
    thread_create(stack, STACK_SIZE, PRIORITY, THREAD_CREATE_STACKTEST, accelerometer_thread, NULL, "accelerometer_thread");

    return 0;
}
