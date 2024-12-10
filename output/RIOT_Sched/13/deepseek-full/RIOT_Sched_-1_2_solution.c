
#include "sched.h"
#include "xtimer.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define THREAD_STACKSIZE_DEFAULT (512)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

static char thread_stack1[THREAD_STACKSIZE_DEFAULT];
static char thread_stack2[THREAD_STACKSIZE_DEFAULT];

static void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

static void *thread2_func(void *arg)
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
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(thread_stack1, sizeof(thread_stack1), THREAD_PRIORITY_MAIN,
                         THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    pid2 = thread_create(thread_stack2, sizeof(thread_stack2), THREAD_PRIORITY_MAIN,
                         THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of Thread 2\n");
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN + 1);

    return 0;
}
