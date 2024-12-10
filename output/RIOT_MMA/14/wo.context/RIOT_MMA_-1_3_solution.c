#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "msg.h"
#include "core/thread.h"
#include "ztimer.h"
#include <stdio.h>

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define SLEEP_TIME (5U)

static char thread_stack[STACK_SIZE];
static kernel_pid_t sensor_thread_pid;
static msg_t sensor_msg_queue[8];

static void *sensor_thread(void *arg) {
    msg_init_queue(sensor_msg_queue, 8);
    (void)arg;

    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return NULL;
    }

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, SLEEP_TIME * 1000);

        mma8x5x_data_t data;
        mma8x5x_read(&dev, &data);

        printf("Acceleration: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
    }
    return NULL;
}

int main(void) {
    sensor_thread_pid = thread_create(
        thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, sensor_thread, NULL, "sensor_thread"
    );
    if (sensor_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create sensor thread\n");
        return 1;
    }

    return 0;
}