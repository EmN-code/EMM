#include <thread_config.h>
#include <stdlib.h>
#include <stdio.h>
#include <kernel_defines.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

void *thread_one(void *arg) {
    while (1) {
        // Thread functionality
        printf("Thread one is running with new priority.\n");
    }
    return NULL;
}

void *thread_two(void *arg) {
    while (1) {
        // Thread functionality
        printf("Thread two is running with new priority.\n");
    }
    return NULL;
}

int main(void) {
    char stack_one[THREAD_STACKSIZE];
    char stack_two[THREAD_STACKSIZE];
    kernel_pid_t pid_one, pid_two;
    
    // Create thread one
    pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");
    
    // Create thread two
    pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");

    // Change priority at runtime
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN - 4);
    
    printf("Threads have been created and priorities have been set.\n");
    
    return 0;
}