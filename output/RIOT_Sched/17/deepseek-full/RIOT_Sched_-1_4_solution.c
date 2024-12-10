
#include <stdio.h>
#include <unistd.h>
#include "xtimer.h"
#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread_func1(void *arg) {
    (void)arg;
    printf("Thread 1 started with priority %d\n", sched_active_thread->priority);
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    printf("Thread 2 started with priority %d\n", sched_active_thread->priority);
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    // Create the first thread
    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    if (pid1 < 0) {
        printf("Error creating thread 1\n");
        return 1;
    }

    // Create the second thread
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread_func2, NULL, "thread2");
    if (pid2 < 0) {
        printf("Error creating thread 2\n");
        return 1;
    }

    // Change priority of thread 1 after some time
    xtimer_sleep(5);
    sched_set_priority(pid1, PRIORITY_LOW);
    printf("Changed priority of thread 1 to %d\n", PRIORITY_LOW);

    // Change priority of thread 2 after some time
    xtimer_sleep(5);
    sched_set_priority(pid2, PRIORITY_HIGH);
    printf("Changed priority of thread 2 to %d\n", PRIORITY_HIGH);

    return 0;
}
