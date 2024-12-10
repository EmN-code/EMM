
#include "xtimer.h"
#include "ztimer.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define SLEEP_INTERVAL (5 * US_PER_SEC)  // 5 seconds

static mma8x5x_t dev;
static mma8x5x_data_t data;
static evtimer_msg_t timer;
static msg_t msg_queue[8];

void read_sensor(void)
{
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
    } else {
        puts("Failed to read accelerometer data");
    }
}

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.ptr = NULL;
    msg_send(&msg, (kernel_pid_t)arg);
}

int main(void)
{
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x accelerometer");
        return 1;
    }

    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    evtimer_init_msg(&timer);
    evtimer_add_msg(&timer, SLEEP_INTERVAL, timer_callback, thread_getpid());

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        read_sensor();
        evtimer_add_msg(&timer, SLEEP_INTERVAL, timer_callback, thread_getpid());
    }

    return 0;
}
