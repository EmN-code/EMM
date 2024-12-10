
#include "thread.h"
#include "sched.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "timex.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "xtimer.h"
#include "mma8x5x_regs.h"
#include "periph/i2c.h"
#include "ztimer.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "periph/gpio.h"
#include "freertos/task.h"

#define THREAD_STACKSIZE_DEFAULT (512)

void *thread_func_1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func_2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    char stack_1[THREAD_STACKSIZE_DEFAULT];
    char stack_2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN - 1,
                                       THREAD_CREATE_WOUT_YIELD, thread_func_1, NULL, "thread_1");

    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN - 2,
                                       THREAD_CREATE_WOUT_YIELD, thread_func_2, NULL, "thread_2");

    xtimer_sleep(5);

    thread_t *thread_1 = thread_get_from_pid(pid_1);
    thread_t *thread_2 = thread_get_from_pid(pid_2);

    sched_change_priority(thread_1, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_2, THREAD_PRIORITY_MAIN - 1);

    return 0;
}
