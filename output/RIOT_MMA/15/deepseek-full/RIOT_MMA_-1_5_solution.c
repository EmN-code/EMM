
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <dht.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <dht_params.h>
#include <stdlib.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <unistd.h>
#include <stdatomic.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define MSG_QUEUE_SIZE 8

static mma8x5x_t dev;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void accelerometer_thread(void) {
    mma8x5x_data_t data;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x accelerometer failed");
        return 1;
    }

    thread_t *acc_thread;
    acc_thread = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        accelerometer_thread,
        NULL,
        "accelerometer_thread"
    );

    if (acc_thread == NULL) {
        puts("Failed to create accelerometer thread");
        return 1;
    }

    return 0;
}
