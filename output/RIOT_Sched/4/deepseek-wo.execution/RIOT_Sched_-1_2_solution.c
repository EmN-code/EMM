
#include "evtimer_msg.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "sched.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "xtimer.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

char stack1[STACKSIZE];
char stack2[STACKSIZE];

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

    pid1 = thread_create(stack1, STACKSIZE, PRIORITY_HIGH, THREAD_CREATE_WOUT_YIELD, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, STACKSIZE, PRIORITY_LOW, THREAD_CREATE_WOUT_YIELD, thread_func2, NULL, "thread2");

    ztimer_sleep(ZTIMER_MSEC, 5000);

    sched_change_priority(thread_get(pid1), PRIORITY_LOW);
    sched_change_priority(thread_get(pid2), PRIORITY_HIGH);

    return 0;
}
