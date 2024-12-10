
#include "ztimer.h"
#include "xtimer.h"
#include "msg.h"
#include "event/thread.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "thread.h"

#define STACK_SIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

static char stack1[STACK_SIZE];
static char stack2[STACK_SIZE];

static void *thread_func1(void *arg)
{
    (void)arg;
    printf("Thread 1 started with priority %d\n", sched_active_thread->priority);
    xtimer_sleep(5);
    printf("Thread 1 changing priority to %d\n", PRIORITY_LOW);
    sched_set_priority(sched_active_pid(), PRIORITY_LOW);
    printf("Thread 1 new priority: %d\n", sched_active_thread->priority);
    return NULL;
}

static void *thread_func2(void *arg)
{
    (void)arg;
    printf("Thread 2 started with priority %d\n", sched_active_thread->priority);
    xtimer_sleep(5);
    printf("Thread 2 changing priority to %d\n", PRIORITY_HIGH);
    sched_set_priority(sched_active_pid(), PRIORITY_HIGH);
    printf("Thread 2 new priority: %d\n", sched_active_thread->priority);
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    printf("Main thread started\n");

    return 0;
}
