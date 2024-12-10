
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "ztimer.h"
#include "mbox.h"
#include "xtimer.h"
#include "unistd.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "timex.h"
#include "time_units.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

char stack1[STACKSIZE];
char stack2[STACKSIZE];

kernel_pid_t pid1, pid2;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", sched_active_thread->priority);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", sched_active_thread->priority);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void)
{
    pid1 = thread_create(stack1, STACKSIZE, PRIORITY_HIGH, THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");
    pid2 = thread_create(stack2, STACKSIZE, PRIORITY_LOW, THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    ztimer_sleep(ZTIMER_MSEC, 5000);

    sched_change_priority(pid1, PRIORITY_LOW);
    sched_change_priority(pid2, PRIORITY_HIGH);

    return 0;
}
