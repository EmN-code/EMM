
#include <stdio.h>
#include <unistd.h>
#include "thread.h"
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"

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
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void change_priority(kernel_pid_t pid, uint8_t new_priority)
{
    sched_set_status(pid, STATUS_SLEEPING);
    sched_set_priority(pid, new_priority);
    sched_set_status(pid, STATUS_RUNNING);
}

int main(void)
{
    puts("Starting two threads with priority change at runtime");

    pid1 = thread_create(stack1, STACKSIZE, PRIORITY_HIGH, THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");
    pid2 = thread_create(stack2, STACKSIZE, PRIORITY_LOW, THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    puts("Changing priorities...");
    change_priority(pid1, PRIORITY_LOW);
    change_priority(pid2, PRIORITY_HIGH);

    puts("Priorities changed. Threads will continue running with new priorities.");

    return 0;
}
