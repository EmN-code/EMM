
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread.h>
#include <msg.h>
#include <ztimer.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>

#define STACKSIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MSG_QUEUE_SIZE (8)

static char stack[STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];
static mma8x5x_t dev;
static mma8x5x_data_t data;

void accelerometer_task(void *arg)
{
    (void)arg;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_SEC, 5);
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

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer: %d\n", res);
        return 1;
    }

    thread_create(stack, sizeof(stack), PRIORITY, 0, accelerometer_task, NULL, "accelerometer_task");

    return 0;
}
