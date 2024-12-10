/* Description: This RIOT application is designed to run on an ESP32 microcontroller. It repeatedly reads data from an MMA8x5x accelerometer at 5-second intervals and displays the results. Required libraries are imported, and we initialize the accelerometer and create a thread to handle periodic data acquisition. */

#include "ztimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "unistd.h"
#include "thread_arch.h"
#include "thread.h"
#include "time_units.h"
#include "timex.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mqttclient.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "dht_params.h"
#include "dht.h"
#include "architecture.h"
#include "paho_mqtt.h"

#define MAIN_QUEUE_SIZE     (8)
#define SLEEP_TIME          (5U * US_PER_SEC)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_accelerometer(void *arg) {
    (void)arg;
    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_read(&dev, &data);
            printf("Accelerometer Data: X: %d mg, Y: %d mg, Z: %d mg\n", data.x_axis, data.y_axis, data.z_axis);
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error: unable to initialize accelerometer\n");
        return 1;
    }

    char stack[THREAD_STACKSIZE_MAIN];

    kernel_pid_t thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST, read_accelerometer, NULL, "accelerometer");

    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error: unable to create thread\n");
        return 1;
    }

    return 0;
}