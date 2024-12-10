
#include <stdio.h>
#include <unistd.h>
#include "thread.h"
#include "xtimer.h"
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

kernel_pid_t pid1, pid2;

void *thread_func1(void *arg)
{
    (void)arg;
    printf("Thread 1 started\n");
    xtimer_sleep(5);
    printf("Thread 1 finished\n");
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    printf("Thread 2 started\n");
    xtimer_sleep(5);
    printf("Thread 2 finished\n");
    return NULL;
}

int main(void)
{
    printf("Starting main thread\n");

    // Create the first thread
    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    if (pid1 <= KERNEL_PID_UNDEF) {
        printf("Error creating thread 1\n");
        return 1;
    }

    // Create the second thread
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread_func2, NULL, "thread2");
    if (pid2 <= KERNEL_PID_UNDEF) {
        printf("Error creating thread 2\n");
        return 1;
    }

    // Wait for both threads to finish
    thread_sleep();

    // Change the priority of the first thread
    thread_set_priority(pid1, PRIORITY_LOW);
    printf("Priority of thread 1 changed to low\n");

    // Change the priority of the second thread
    thread_set_priority(pid2, PRIORITY_HIGH);
    printf("Priority of thread 2 changed to high\n");

    printf("Main thread finished\n");
    return 0;
}
