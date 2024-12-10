#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <msg.h>
#include <mbox.h>
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "periph_conf.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

#define STACKSIZE_THREAD        (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_THREAD         (THREAD_PRIORITY_MAIN - 1)

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void *accel_thread(void *arg) {
    (void)arg;
    while(1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration [mg]: X=%d Y=%d Z=%d\n", data.x_axis, data.y_axis, data.z_axis);
        sleep(5);
    }
    return NULL;
}

int main(void) {
    int res;

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS, // Adjusted macro value name
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x\n");
        return 1;
    }

    char *stack = (char *)malloc(STACKSIZE_THREAD);
    if (stack == NULL) {
        puts("Failed to allocate memory for thread stack\n");
        return 1;
    }

    kernel_pid_t pid = thread_create(stack, STACKSIZE_THREAD, PRIORITY_THREAD, THREAD_CREATE_STACKTEST, accel_thread, NULL, "accel_thread");
    if (pid <= KERNEL_PID_UNDEF) {
        free(stack);
        puts("Failed to create accelerometer thread\n");
        return 1;
    }
    return 0;
}