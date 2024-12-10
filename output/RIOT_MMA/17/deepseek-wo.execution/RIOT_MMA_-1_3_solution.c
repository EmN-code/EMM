
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "mbox.h"
#include "evtimer_msg.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void _read_accelerometer(void)
{
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read accelerometer data\n");
    }
}

static void _timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0;
    msg_send(&msg, (kernel_pid_t)arg);
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    ztimer_t timer = {
        .callback = _timer_callback,
        .arg = (void *)thread_getpid(),
    };

    while (1) {
        ztimer_set(ZTIMER_SEC, &timer, 5);
        msg_t msg;
        msg_receive(&msg);
        _read_accelerometer();
    }

    return 0;
}
