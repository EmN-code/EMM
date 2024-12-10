#include "thread.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "msg.h"
#include <stdio.h>

static kernel_pid_t thread_pid;
static char thread_stack[THREAD_STACKSIZE_MAIN];

static void* mma8x5x_thread(void *arg) {
    (void)arg;
    msg_t msg;

    i2c_t i2c_dev = I2C_DEV(0);
    mma8x5x_params_t params = {
        .i2c = i2c_dev,
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    mma8x5x_t dev;
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("MMA8x5x initialization failed");
        return NULL;
    }
    
    while (1) {
        msg_receive(&msg);
        mma8x5x_data_t data;
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST,
                               mma8x5x_thread, NULL, "mma8x5x_thread");
    if (thread_pid <= KERNEL_PID_UNDEF) {
        puts("Error initializing the test application\n");
        return 1;
    }
    return 0;
}