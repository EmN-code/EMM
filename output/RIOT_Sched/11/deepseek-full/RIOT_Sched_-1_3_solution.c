
#include "sched.h"
#include "xtimer.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define STACKSIZE 1024
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 15

kernel_pid_t pid1, pid2;

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg)
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
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread_func2, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of thread 1 to LOW\n");
    sched_change_priority(pid1, PRIORITY_LOW);

    xtimer_sleep(5);

    printf("Changing priority of thread 2 to HIGH\n");
    sched_change_priority(pid2, PRIORITY_HIGH);

    return 0;
}
