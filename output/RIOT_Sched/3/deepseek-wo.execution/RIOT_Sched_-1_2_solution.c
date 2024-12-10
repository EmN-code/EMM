
#include <stdio.h>
#include <inttypes.h>
#include "thread.h"
#include "sched.h"
#include "event/thread.h"
#include "event/periodic_callback.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

char stack1[STACKSIZE];
char stack2[STACKSIZE];

kernel_pid_t pid1, pid2;

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 started with priority %" PRIu8 "\n", sched_active_thread->priority);
    while (1) {
        printf("Thread 1 running\n");
        thread_sleep(1000000); // Sleep for 1 second
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 started with priority %" PRIu8 "\n", sched_active_thread->priority);
    while (1) {
        printf("Thread 2 running\n");
        thread_sleep(1000000); // Sleep for 1 second
    }
    return NULL;
}

int main(void)
{
    puts("RIOT multi-threading example");

    // Create the first thread with high priority
    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    // Create the second thread with low priority
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Change the priority of the first thread to low
    sched_change_priority(thread_get(pid1), PRIORITY_LOW);
    printf("Changed priority of Thread 1 to %" PRIu8 "\n", PRIORITY_LOW);

    // Change the priority of the second thread to high
    sched_change_priority(thread_get(pid2), PRIORITY_HIGH);
    printf("Changed priority of Thread 2 to %" PRIu8 "\n", PRIORITY_HIGH);

    return 0;
}
