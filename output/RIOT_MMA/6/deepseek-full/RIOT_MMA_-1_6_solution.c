
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "ztimer.h"
#include "thread.h"
#include "msg.h"

#define STACKSIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MSG_QUEUE_SIZE (8)

static char thread_stack[STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

static void *sensor_thread(void *arg)
{
    (void)arg;

    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("MMA8x5x initialization failed");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_create(thread_stack, sizeof(thread_stack), PRIORITY, 0, sensor_thread, NULL, "sensor_thread");

    return 0;
}
