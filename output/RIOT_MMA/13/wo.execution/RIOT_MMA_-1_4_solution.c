#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "timex.h"
#include "stdio.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "dht_params.h"
#include "MQTTClient.h"

#define MAIN_QUEUE_SIZE    (8)
#define SLEEP_TIME_S       (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
mma8x5x_t dev;
mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_accelerometer(void *arg) {
    (void)arg;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accel Data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_SEC, SLEEP_TIME_S);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Init failed\n");
        return 1;
    }
    printf("MMA8x5x init successful\n");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_accelerometer, NULL,
                  "accel_read_thread");

    return 0;
}