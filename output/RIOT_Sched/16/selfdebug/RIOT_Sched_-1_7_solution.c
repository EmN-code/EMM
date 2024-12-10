#include "thread_arch.h"
#include "kernel_defines.h"
#include <stdio.h>

// Define stack size for threads
#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

// Function that will be executed by thread 1
void *thread1(void *arg) {
    printf("Thread 1 started\n");
    while (1) {
        // Simulate some work
    }
    return NULL;
}

// Function that will be executed by thread 2
void *thread2(void *arg) {
    printf("Thread 2 started\n");
    while (1) {
        // Simulate some work
    }
    return NULL;
}

int main(void) {
    char thread1_stack[THREAD_STACKSIZE];
    char thread2_stack[THREAD_STACKSIZE];

    // Create two threads
    kernel_pid_t pid1 = thread_create(thread1_stack, sizeof(thread1_stack), THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST, thread1, NULL, "thread1");
    
    kernel_pid_t pid2 = thread_create(thread2_stack, sizeof(thread2_stack), THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST, thread2, NULL, "thread2");

    // Change priority of thread 1
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 2);

    // Change priority of thread 2
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN);

    // To avoid the unused variable warning, print the PID
    printf("Thread 1 PID: %d\n", pid1);
    printf("Thread 2 PID: %d\n", pid2);

    return 0;
}