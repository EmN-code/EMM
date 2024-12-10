#include <stdio.h>
#include <stdlib.h>
#include "thread_arch.h"
#include "thread_config.h"

void *thread1_handler(void *arg) {
    printf("Thread 1 is running with default priority.\n");
    while (1) {
        // Simulate workload
    }
    return NULL;
}

void *thread2_handler(void *arg) {
    printf("Thread 2 is running with default priority.\n");
    while (1) {
        // Simulate workload
    }
    return NULL;
}

int main(void) {
    printf("Starting threads...\n");
    
    // Create two threads
    kernel_pid_t thread1 = thread_create(NULL, 
                                         THREAD_PRIORITY_MAIN - 1, 
                                         THREAD_CREATE_STACKTEST, 
                                         thread1_handler, NULL, 
                                         "thread1");
                                         
    kernel_pid_t thread2 = thread_create(NULL, 
                                         THREAD_PRIORITY_MAIN - 1, 
                                         THREAD_CREATE_STACKTEST, 
                                         thread2_handler, NULL, 
                                         "thread2");

    // Print before changing priority
    printf("Changing priorities of threads...\n");

    // Change priority of thread1
    sched_change_priority(thread1, THREAD_PRIORITY_MAIN - 2);
    printf("Thread 1 priority changed.\n");

    // Change priority of thread2
    sched_change_priority(thread2, THREAD_PRIORITY_MAIN - 3);
    printf("Thread 2 priority changed.\n");

    // Keep main thread alive
    while (1) {
        // Prevent main from exiting
    }
    
    return 0;  // Although it won't reach here
}