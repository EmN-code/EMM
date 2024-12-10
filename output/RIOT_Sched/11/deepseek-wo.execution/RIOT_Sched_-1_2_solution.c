
#include "ztimer.h"
#include "xtimer.h"
#include "event/thread.h"
#include "timex.h"
#include "sched.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "thread.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", sched_get_priority(thread_getpid()));
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", sched_get_priority(thread_getpid()));
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void)
{
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    thread1_pid = thread_create(stack1, STACKSIZE, PRIORITY_HIGH, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, STACKSIZE, PRIORITY_LOW, 0, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of thread1 to LOW\n");
    sched_change_priority(thread1_pid, PRIORITY_LOW);

    xtimer_sleep(5);

    printf("Changing priority of thread2 to HIGH\n");
    sched_change_priority(thread2_pid, PRIORITY_HIGH);

    return 0;
}
