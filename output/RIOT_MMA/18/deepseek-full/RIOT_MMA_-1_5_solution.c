
#include "ztimer.h"
#include "msg.h"
#include "thread.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static kernel_pid_t _sensor_thread_pid;

static void *sensor_thread(void *arg)
{
    (void)arg;
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("MMA8x5x initialization failed");
        return NULL;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Accelerometer data: x: %d, y: %d, z: %d\n",
                   data.x, data.y, data.z);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    _sensor_thread_pid = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sensor_thread,
        NULL,
        "sensor_thread"
    );

    if (_sensor_thread_pid <= KERNEL_PID_UNDEF) {
        puts("Failed to create sensor thread");
        return 1;
    }

    puts("Sensor thread started");

    return 0;
}
