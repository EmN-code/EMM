
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <sched.h>

#define STACK_SIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

void *thread_func_1(void *arg) {
    printf("Thread 1 started with priority %d\n", sched_get_priority(thread_get_active()));
    while (1) {
        printf("Thread 1 running\n");
        // Simulate some work
        for (volatile int i = 0; i < 1000000; i++);
    }
    return NULL;
}

void *thread_func_2(void *arg) {
    printf("Thread 2 started with priority %d\n", sched_get_priority(thread_get_active()));
    while (1) {
        printf("Thread 2 running\n");
        // Simulate some work
        for (volatile int i = 0; i < 1000000; i++);
    }
    return NULL;
}

int main(void) {
    char stack_1[STACK_SIZE];
    char stack_2[STACK_SIZE];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), PRIORITY_HIGH, THREAD_CREATE_STACKTEST, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), PRIORITY_LOW, THREAD_CREATE_STACKTEST, thread_func_2, NULL, "thread_2");

    printf("Main thread started\n");

    // Change priority of thread 1 to low after some time
    for (volatile int i = 0; i < 5000000; i++);
    sched_change_priority(thread_get(pid_1), PRIORITY_LOW);
    printf("Changed priority of Thread 1 to %d\n", PRIORITY_LOW);

    // Change priority of thread 2 to high after some time
    for (volatile int i = 0; i < 5000000; i++);
    sched_change_priority(thread_get(pid_2), PRIORITY_HIGH);
    printf("Changed priority of Thread 2 to %d\n", PRIORITY_HIGH);

    while (1) {
        // Main thread doing some work
        for (volatile int i = 0; i < 1000000; i++);
    }

    return 0;
}
