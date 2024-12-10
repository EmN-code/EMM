#include <stdio.h>  // Required for printf
#include "kernel_defines.h"  // Required for kernel constants and functions

void *thread_one(void *arg) {
    printf("Thread one is running\n");
    return NULL;
}

void *thread_two(void *arg) {
    printf("Thread two is running\n");
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t pid1, pid2;

    // Create and launch two threads
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_one, NULL, "thread_one");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, 0, thread_two, NULL, "thread_two");

    // Log thread creation
    printf("Created thread one with PID %d\n", pid1);
    printf("Created thread two with PID %d\n", pid2);

    // Change thread priorities
    int new_priority_1 = THREAD_PRIORITY_MAIN - 2;
    int new_priority_2 = THREAD_PRIORITY_MAIN - 3;

    sched_change_priority((kernel_pid_t)pid1, new_priority_1);
    sched_change_priority((kernel_pid_t)pid2, new_priority_2);

    printf("Changed priority of thread one to %d\n", new_priority_1);
    printf("Changed priority of thread two to %d\n", new_priority_2);

    return 0;
}