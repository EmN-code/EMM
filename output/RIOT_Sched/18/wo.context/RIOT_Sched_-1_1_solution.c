#include <thread_arch.h>
#include <thread_config.h>
#include <stdio.h>
#include <stdlib.h>

void* thread_func_1(void *arg) {
    printf("Thread 1 is running with initial priority.\n");
    thread_yield();  // Allow other threads to run
    printf("Thread 1 finished execution.\n");
    return NULL;
}

void* thread_func_2(void *arg) {
    printf("Thread 2 is running with initial priority.\n");
    thread_yield();  // Allow other threads to run
    printf("Thread 2 finished execution.\n");
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    // Create thread 1
    if ((pid1 = thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_func_1, NULL, "thread_1")) <= KERNEL_PID_UNDEF) {
        printf("Error creating thread 1\n");
        return 1;
    }

    // Create thread 2
    if ((pid2 = thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, thread_func_2, NULL, "thread_2")) <= KERNEL_PID_UNDEF) {
        printf("Error creating thread 2\n");
        return 1;
    }

    // Change priority of thread 1 at runtime
    if (sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 3) != 0) {
        printf("Error changing priority of thread 1\n");
    }

    // Change priority of thread 2 at runtime
    if (sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 4) != 0) {
        printf("Error changing priority of thread 2\n");
    }

    // Wait for both threads to finish execution
    while (thread_get_status(pid1) != STATUS_TERMINATED || thread_get_status(pid2) != STATUS_TERMINATED) {
        thread_yield();
    }

    printf("Both threads have completed execution.\n");
    return 0;
}