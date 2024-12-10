
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "thread.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

static kernel_pid_t pid1, pid2;

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
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread1_func, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of Thread 2\n");
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN + 1);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
