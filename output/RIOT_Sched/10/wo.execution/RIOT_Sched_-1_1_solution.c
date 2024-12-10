#include "thread_config.h"
#include "hal/gpio_types.h"
#include "architecture.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define stack size for threads
#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 5

// Define thread function for thread1
void *thread1_func(void *arg) {
    printf("Thread 1 is running.\n");
    while (1) {
        // Do some work
    }
    return NULL;
}

// Define thread function for thread2
void *thread2_func(void *arg) {
    printf("Thread 2 is running.\n");
    while (1) {
        // Do some work
    }
    return NULL;
}

int main(void) {
    // Allocate memory for thread stacks
    char thread1_stack[THREAD_STACKSIZE_MAIN];
    char thread2_stack[THREAD_STACKSIZE_MAIN];

    // Create thread 1
    kernel_pid_t thread1_pid = thread_create(thread1_stack, THREAD_STACKSIZE_MAIN,
                                             THREAD_PRIORITY_MAIN, 0,
                                             thread1_func, NULL, "thread1");
    if (thread1_pid < 0) {
        printf("Error creating thread 1.\n");
        return 1;
    }

    // Create thread 2
    kernel_pid_t thread2_pid = thread_create(thread2_stack, THREAD_STACKSIZE_MAIN,
                                             THREAD_PRIORITY_MAIN + 1, 0,
                                             thread2_func, NULL, "thread2");
    if (thread2_pid < 0) {
        printf("Error creating thread 2.\n");
        return 1;
    }

    // Give time for threads to start
    xtimer_sleep(2);

    // Change priority of thread 1 to higher than thread 2
    int res = sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 1);
    if (res < 0) {
        printf("Error changing priority of thread 1.\n");
        return 1;
    }

    printf("Priority of thread 1 has been raised.\n");

    while (1) {
        // Main thread can do other work here...
    }

    return 0;
}