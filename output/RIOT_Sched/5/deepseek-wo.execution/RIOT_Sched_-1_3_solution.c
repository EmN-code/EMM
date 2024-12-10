
#include "evtimer_msg.h"
#include "timex.h"
#include "kernel_defines.h"
#include "thread.h"
#include "ztimer.h"
#include "sched.h"
#include "event/thread.h"
#include "assert.h"
#include "mma8x5x_params.h"
#include "event/periodic_callback.h"
#include "dht_params.h"
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "time_units.h"

#define THREAD_STACKSIZE_DEFAULT (512)

static char stack1[THREAD_STACKSIZE_DEFAULT];
static char stack2[THREAD_STACKSIZE_DEFAULT];

void thread1_func(void)
{
    while (1) {
        printf("Thread 1 is running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
}

void thread2_func(void)
{
    while (1) {
        printf("Thread 2 is running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, (thread_task_func_t)thread1_func, NULL, "thread1");

    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                         THREAD_CREATE_STACKTEST, (thread_task_func_t)thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 1);

    return 0;
}
