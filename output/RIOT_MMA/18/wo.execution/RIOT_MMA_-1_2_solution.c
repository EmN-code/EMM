#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "periph_conf.h"

#include <stdio.h>

static kernel_pid_t thread_pid;
static char stack[THREAD_STACKSIZE_MAIN];

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

static void *read_sensor(void *arg) {
    (void)arg; // Suppress unused parameter warning

    // Initialize the accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer\n");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    printf("Starting accelerometer reading thread\n");
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST, read_sensor, NULL, "read_sensor");

    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating thread\n");
    }

    return 0;
}