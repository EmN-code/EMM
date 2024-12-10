#include <stdatomic.h>
#include <MQTTClient.h>
#include <evtimer_mbox.h>
#include <mma8x5x_regs.h>
#include <paho_mqtt.h>
#include <stdbool.h>
#include <ztimer.h>
#include <msg.h>
#include <dht_params.h>
#include <timex.h>
#include <dht.h>
#include <stddef.h>
#include <stdio.h>
#include <kernel_defines.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <time_units.h>
#include <periph/i2c.h>
#include <periph_conf.h>
#include <stdlib.h>
#include <xtimer.h>
#include <mma8x5x_params.h>
#include <thread.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <event/thread.h>

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN -4)
#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static kernel_pid_t pid;

static mma8x5x_t acc_dev;
static mma8x5x_data_t acc_data;
mma8x5x_params_t acc_params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

static void *thread_handler(void *arg) {
    (void)arg;

    // Initiate MMA8x5x
    int init_res = mma8x5x_init(&acc_dev, &acc_params);
    while (init_res != MMA8X5X_OK) {
        printf("MMA8x5x initialization failed: %d. Retrying...\n", init_res);
        xtimer_sleep(1);
        init_res = mma8x5x_init(&acc_dev, &acc_params);
    }
    printf("MMA8x5x initialization successful.\n");

    // Periodically read from MMA8x5x
    while (1) {
        mma8x5x_read(&acc_dev, &acc_data);
        printf("Acceleration: X=%d mg, Y=%d mg, Z=%d mg\n",
               acc_data.x_axis, acc_data.y_axis, acc_data.z_axis);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);
    pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, thread_handler, NULL, "mma8x5x_thread");
    printf("Thread created with PID %d\n", pid);

    return 0;
}