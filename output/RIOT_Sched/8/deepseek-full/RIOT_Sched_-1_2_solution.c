
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <evtimer_mbox.h>
#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <ztimer.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>

#define STACKSIZE 1024
#define PRIORITY 5

char stack1[STACKSIZE];
char stack2[STACKSIZE];

kernel_pid_t pid1, pid2;

void *thread1(void *arg)
{
    (void)arg;
    printf("Thread 1 started with PID: %d\n", thread_getpid());
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2(void *arg)
{
    (void)arg;
    printf("Thread 2 started with PID: %d\n", thread_getpid());
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    printf("Starting main thread\n");

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY, 0, thread1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY, 0, thread2, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of thread 1\n");
    thread_change_priority(pid1, PRIORITY + 1);

    xtimer_sleep(5);

    printf("Changing priority of thread 2\n");
    thread_change_priority(pid2, PRIORITY + 1);

    while (1) {
        printf("Main thread running\n");
        xtimer_sleep(5);
    }

    return 0;
}
