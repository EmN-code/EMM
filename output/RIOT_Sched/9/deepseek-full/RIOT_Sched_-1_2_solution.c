
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "sched.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define THREAD_STACKSIZE_DEFAULT (512)

static char thread_stack1[THREAD_STACKSIZE_DEFAULT];
static char thread_stack2[THREAD_STACKSIZE_DEFAULT];

static void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

static void *thread_func2(void *arg)
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
    uint8_t priority1, priority2;

    pid1 = thread_create(thread_stack1, sizeof(thread_stack1),
                         THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_WOUT_YIELD,
                         thread_func1, NULL, "thread1");

    pid2 = thread_create(thread_stack2, sizeof(thread_stack2),
                         THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_WOUT_YIELD,
                         thread_func2, NULL, "thread2");

    priority1 = thread_get_priority(thread_get(pid1));
    priority2 = thread_get_priority(thread_get(pid2));

    printf("Initial priorities: Thread 1: %u, Thread 2: %u\n", priority1, priority2);

    xtimer_sleep(5);

    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 1);

    priority1 = thread_get_priority(thread_get(pid1));
    priority2 = thread_get_priority(thread_get(pid2));

    printf("Changed priorities: Thread 1: %u, Thread 2: %u\n", priority1, priority2);

    return 0;
}
