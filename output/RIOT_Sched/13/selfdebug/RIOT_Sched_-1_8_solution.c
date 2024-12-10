#include "thread.h"
#include "sched.h"
#include <stdio.h>

#define STACK_SIZE (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)
char stack1[STACK_SIZE];
char stack2[STACK_SIZE];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running with priority %d\n", THREAD_PRIORITY_MAIN - 1);
        thread_yield();
    }
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running with priority %d\n", THREAD_PRIORITY_MAIN - 2);
        thread_yield();
    }
    return NULL;
}

int main(void) {
    puts("Thread priority example started\n");
    
    // Create threads
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Change priority of thread1 at runtime
    printf("Changing priority of thread1\n");
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 3);

    // Main thread loop
    while (1) {
        printf("Main thread running\n");
        thread_yield();
    }

    return 0;
}