#include <ztimer.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <architecture.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <mbox.h>
#include <thread.h>
#include <event/thread.h>
#include <timex.h>
#include <time_units.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <periph_conf.h>
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "dht_params.h"
#include "dht.h"

#define ADDR 0x1D
#ifndef THREAD_STACKSIZE_DEFAULT
#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN)
#endif

#ifndef THREAD_PRIORITY_MAIN
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_DEFAULT)
#endif

static mma8x5x_t dev;
static mma8x5x_data_t data;

static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_accelerometer(void *arg) {
    (void)arg;

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_DEFAULT];

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                                     read_accelerometer, NULL, "read_accelerometer");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    while (1) {
        // Main thread loop can perform other tasks or stay idle
        ztimer_sleep(ZTIMER_MSEC, 2000);
    }

    return 0;
}