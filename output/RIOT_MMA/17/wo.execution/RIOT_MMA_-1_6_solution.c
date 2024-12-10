#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <kernel_defines.h>
#include <thread_arch.h>
#include <thread.h>
#include <architecture.h>
#include <time_units.h>
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "event/thread.h"
#include "xtimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <mbox.h>

#define SLEEP_DURATION (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];
static kernel_pid_t thread_pid;
mma8x5x_t dev;

void *accelerometer_thread(void *arg) {
    (void)arg;
    mma8x5x_data_t data;

    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_read(&dev, &data);
            printf("Acceleration [mg]: X: %d, Y: %d, Z: %d\n",
                   data.x_axis, data.y_axis, data.z_axis);
        }
        ztimer_sleep(ZTIMER_USEC, SLEEP_DURATION);
    }

    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize accelerometer");
        return 1;
    }

    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST, accelerometer_thread,
                               NULL, "accelerometer_thread");
    if (thread_pid <= 0) {
        puts("Failed to create thread");
        return 1;
    }

    return 0;
}