
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

#define STACKSIZE 512
#define PRIORITY 5
#define MSG_QUEUE_SIZE 16

static char stack[STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

void event_loop(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Message received in event loop\n");
    }
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    kernel_pid_t pid = thread_create(stack, sizeof(stack), PRIORITY, 0, event_loop, NULL, "event_loop");
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create event loop thread\n");
        return 1;
    }

    mma8x5x_data_t data;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}
