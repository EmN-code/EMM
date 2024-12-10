
#include <stdio.h>
#include <thread.h>
#include <ztimer.h>
#include <event/thread.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>

#define STACKSIZE 1024
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

static void *thread1(void *arg)
{
    (void)arg;
    printf("Thread 1 started with priority %d\n", sched_active_thread->priority);
    ztimer_sleep(ZTIMER_MSEC, 1000);
    printf("Thread 1 changing priority to %d\n", PRIORITY_LOW);
    sched_change_priority(sched_active_thread, PRIORITY_LOW);
    ztimer_sleep(ZTIMER_MSEC, 1000);
    printf("Thread 1 finished\n");
    return NULL;
}

static void *thread2(void *arg)
{
    (void)arg;
    printf("Thread 2 started with priority %d\n", sched_active_thread->priority);
    ztimer_sleep(ZTIMER_MSEC, 1000);
    printf("Thread 2 changing priority to %d\n", PRIORITY_HIGH);
    sched_change_priority(sched_active_thread, PRIORITY_HIGH);
    ztimer_sleep(ZTIMER_MSEC, 1000);
    printf("Thread 2 finished\n");
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread2, NULL, "thread2");

    printf("Main thread finished\n");
    return 0;
}
