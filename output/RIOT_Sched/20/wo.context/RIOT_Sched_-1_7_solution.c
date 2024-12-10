#include <thread_config.h>
#include <stdlib.h>
#include <stdio.h>
#include "kernel_defines.h"

// Function declarations
void *thread_one(void *arg);
void *thread_two(void *arg);

int main(void) {
    // Stack sizes for threads
    char stack_one[THREAD_STACKSIZE_DEFAULT];
    char stack_two[THREAD_STACKSIZE_DEFAULT];

    // Create the first thread
    kernel_pid_t pid_one = thread_create(stack_one, sizeof(stack_one), 
                                         THREAD_PRIORITY_MAIN - 1, 
                                         THREAD_CREATE_STACKTEST, 
                                         thread_one, NULL, "thread_one");
    
    // Create the second thread
    kernel_pid_t pid_two = thread_create(stack_two, sizeof(stack_two), 
                                         THREAD_PRIORITY_MAIN - 2, 
                                         THREAD_CREATE_STACKTEST, 
                                         thread_two, NULL, "thread_two");

    // Change the priority of the threads at runtime
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN - 1);

    return 0;
}

void *thread_one(void *arg) {
    while (1) {
        // Thread functionality
    }
    return NULL;
}

void *thread_two(void *arg) {
    while (1) {
        // Thread functionality
    }
    return NULL;
}