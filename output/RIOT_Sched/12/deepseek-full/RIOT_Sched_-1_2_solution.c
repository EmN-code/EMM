
#include "evtimer_mbox.h"
#include "sched.h"
#include "xtimer.h"
#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define THREAD_STACKSIZE_DEFAULT (1024)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

static char stack1[THREAD_STACKSIZE_DEFAULT];
static char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0, thread_func2, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of thread 1\n");
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN + 1);

    xtimer_sleep(5);

    printf("Changing priority of thread 2\n");
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN + 1);

    return 0;
}