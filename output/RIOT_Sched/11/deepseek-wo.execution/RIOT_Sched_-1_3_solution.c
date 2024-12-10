
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "mbox.h"
#include "time_units.h"
#include "event/thread.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "xtimer.h"

#define STACKSIZE 1024
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        // Thread 1 logic
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        // Thread 2 logic
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void)
{
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    thread1_pid = thread_create(stack1, STACKSIZE, PRIORITY_LOW, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, STACKSIZE, PRIORITY_LOW, 0, thread2_func, NULL, "thread2");

    // Change priority of thread1 to high
    sched_change_priority(thread_get(thread1_pid), PRIORITY_HIGH);

    // Change priority of thread2 to high
    sched_change_priority(thread_get(thread2_pid), PRIORITY_HIGH);

    return 0;
}
