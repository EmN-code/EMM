
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <kernel_defines.h>
#include <stddef.h>
#include <time_units.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <unistd.h>
#include <dht.h>
#include <dht_params.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <event/thread.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define MSG_QUEUE_SIZE 16
#define SLEEP_INTERVAL (5 * US_PER_SEC)

static mma8x5x_t dev;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void accelerometer_task(void) {
    mma8x5x_data_t data;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data: x: %d, y: %d, z: %d\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL);
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
        puts("Failed to initialize MMA8x5x accelerometer");
        return 1;
    }

    thread_t *acc_thread = thread_create(
        (char *)malloc(THREAD_STACKSIZE_DEFAULT),
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        (thread_task_func_t)accelerometer_task,
        NULL,
        "accelerometer_task"
    );

    if (acc_thread == NULL) {
        puts("Failed to create accelerometer task");
        return 1;
    }

    return 0;
}
