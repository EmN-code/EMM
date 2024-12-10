
#include "periph/i2c.h"
#include "ztimer.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "periph/rtc.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "mbox.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "sched.h"
#include "event/periodic_callback.h"

#define THREAD_STACKSIZE_MAIN   (1024)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN)

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0, thread_func2, NULL, "thread2");

    ztimer_sleep(ZTIMER_MSEC, 5000);

    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN + 1);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN + 1);

    return 0;
}
