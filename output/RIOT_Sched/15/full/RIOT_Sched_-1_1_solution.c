// Description: This code initializes two threads on an ESP32 running RIOT OS. It then changes the priority of one of the threads during runtime. Thread functions are defined to simulate basic operations.

// Imports
#include "thread_arch.h"
#include "thread_config.h"
#include <stdio.h>

// Thread stack size and priority definition
#define STACK_SIZE (THREAD_STACKSIZE_MAIN)
#define PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

char stack_one[STACK_SIZE];
char stack_two[STACK_SIZE];

// Thread handler function
void* thread_one_handler(void* arg) {
    printf("Thread One is running\n");
    // Simulate some work...
    return NULL;
}

void* thread_two_handler(void* arg) {
    printf("Thread Two is running\n");
    // Change the priority of this thread to a higher value
    kernel_pid_t pid = thread_getpid();
    sched_change_priority(pid, PRIORITY_MAIN - 1);
    printf("Thread Two new priority set\n");
    return NULL;
}

int main(void) {
    // Initialize threads
    thread_create(stack_one, STACK_SIZE, PRIORITY_MAIN, 0, thread_one_handler, NULL, "ThreadOne");
    thread_create(stack_two, STACK_SIZE, PRIORITY_MAIN, 0, thread_two_handler, NULL, "ThreadTwo");

    // Allow threads to run
    while(1) {
        // Main task can also include other operations
    }

    return 0;
}