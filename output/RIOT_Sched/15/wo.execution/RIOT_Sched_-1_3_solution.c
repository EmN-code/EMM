#include "thread_config.h"
#include "thread_arch.h"
#include "thread.h"
#include "sched.h"
#include "event/thread.h"
#include "architecture.h"
#include "MQTTClient.h"

#include <stdio.h>
#include <stdlib.h>

// Define constants for stack size
#define STACKSIZE_THREAD_ONE (1024)
#define STACKSIZE_THREAD_TWO (1024)

// Define priorities for threads
#define PRIORITY_THREAD_ONE (THREAD_PRIORITY_MAIN + 1)
#define PRIORITY_THREAD_TWO (THREAD_PRIORITY_MAIN + 2)

// Define thread names
#define NAME_THREAD_ONE "Thread 1"
#define NAME_THREAD_TWO "Thread 2"

kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_one(void *arg) {
    printf("Thread 1 running with initial priority\n");
    return NULL;
}

void *thread_two(void *arg) {
    printf("Thread 2 running with initial priority\n");
    return NULL;
}

int main(void) {
    // Allocate stack for threads
    char stack_thread_one[STACKSIZE_THREAD_ONE];
    char stack_thread_two[STACKSIZE_THREAD_TWO];

    // Create thread 1
    thread_one_pid = thread_create(stack_thread_one,
                                   STACKSIZE_THREAD_ONE,
                                   PRIORITY_THREAD_ONE,
                                   CREATE_STACKTEST,
                                   thread_one,
                                   NULL,
                                   NAME_THREAD_ONE);

    // Create thread 2
    thread_two_pid = thread_create(stack_thread_two,
                                   STACKSIZE_THREAD_TWO,
                                   PRIORITY_THREAD_TWO,
                                   CREATE_STACKTEST,
                                   thread_two,
                                   NULL,
                                   NAME_THREAD_TWO);

    // Change priority of thread 1
    sched_change_priority(thread_one_pid, PRIORITY_THREAD_TWO);

    // Change priority of thread 2
    sched_change_priority(thread_two_pid, PRIORITY_THREAD_ONE);

    // Main thread work
    while(1) {
        printf("Main thread running...\n");
        usleep(1000000);
    }

    return 0;
}