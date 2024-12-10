#include "periph/i2c.h"
#include "kernel_defines.h"
#include "msg.h"
#include "fmt.h"
#include "mma8x5x.h"

#define I2C_BUS I2C_DEV(0)
#define MMA8X5X_ADDR 0x1D
#define TIMER_INTERVAL (5U * US_PER_SEC)

static kernel_pid_t runner_pid;
static msg_t msg_queue[1];
static mma8x5x_t dev;

static void timer_cb(void *arg)
{
    (void)arg;
    msg_t msg;
    msg_send(&msg, runner_pid);
}

static void *runner_thread(void *arg)
{
    (void)arg;
    mma8x5x_data_t data;
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (mma8x5x_read(&dev, &data) != 0) {
            printf("Failed to read data\n");
            continue;
        }
        printf("X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
    }
    return NULL;
}

int main(void)
{
    printf("MMA8x5x example application\n");

    mma8x5x_params_t params = {
        .i2c = I2C_BUS,
        .addr = MMA8X5X_ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Sensor initialization failed\n");
        return 1;
    }

    runner_pid = thread_create(
        NULL, THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        runner_thread, NULL, "runner"
    );

    static char timer_stack[THREAD_STACKSIZE_MAIN];
    kernel_pid_t timer_pid = thread_create(
        timer_stack, sizeof(timer_stack),
        THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_STACKTEST,
        (void *(*)(void *))timer_cb, NULL, "timer"
    );

    if (timer_pid <= KERNEL_PID_UNDEF) {
        printf("Timer thread creation failed\n");
        return 1;
    }

    xtimer_ticks_t interval = { .ticks32 = TIMER_INTERVAL / XTIMER_HZ};
    xtimer_periodic_wakeup(&timer_pid, &interval);

    return 0;
}