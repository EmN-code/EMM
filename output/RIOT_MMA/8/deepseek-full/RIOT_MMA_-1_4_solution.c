
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "dht.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "dht_params.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"

#define MSG_QUEUE_SIZE (8)
#define THREAD_STACK_SIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char thread_stack[THREAD_STACK_SIZE];

void sensor_thread(void *arg)
{
    (void)arg;
    mma8x5x_t dev;
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

    msg_t msg;
    while (1) {
        mma8x5x_data_t data;
        mma8x5x_read(&dev, &data);

        msg.content.value = (data.x << 20) | (data.y << 10) | data.z;
        msg_send(&msg, thread_getpid());

        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t sensor_pid = thread_create(thread_stack, sizeof(thread_stack), PRIORITY,
                                            THREAD_CREATE_STACKTEST, sensor_thread, NULL, "sensor_thread");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        mma8x5x_data_t data;
        data.x = (msg.content.value >> 20) & 0x3FF;
        data.y = (msg.content.value >> 10) & 0x3FF;
        data.z = msg.content.value & 0x3FF;

        printf("Acceleration: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
    }

    return 0;
}
