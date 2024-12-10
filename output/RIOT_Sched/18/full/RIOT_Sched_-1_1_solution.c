#include "thread_arch.h"
#include "thread_config.h"
#include <stdio.h>

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];
kernel_pid_t pid1;
kernel_pid_t pid2;

void *thread1_handler(void *arg) {
    printf("Thread 1 is running with priority %d\n", thread_get_priority(pid1));
    while(1) {
        // Simulating some work
    }
    return NULL;
}

void *thread2_handler(void *arg) {
    printf("Thread 2 is running with priority %d\n", thread_get_priority(pid2));
    while(1) {
        // Simulating some work
    }
    return NULL;
}

int main(void) {
    // Start thread 1
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0,
                         thread1_handler, NULL, "thread1");
    printf("Started Thread 1 with PID: %d\n", pid1);

    // Start thread 2
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0,
                         thread2_handler, NULL, "thread2");
    printf("Started Thread 2 with PID: %d\n", pid2);

    // Change priority of thread 1
    if (sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 2) == 0) {
        printf("Changed priority of Thread 1 to: %d\n", THREAD_PRIORITY_MAIN - 2);
    }

    // Change priority of thread 2
    if (sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 1) == 0) {
        printf("Changed priority of Thread 2 to: %d\n", THREAD_PRIORITY_MAIN - 1);
    }

    while(1) {
        // Main thread loop
    }
    return 0;
}