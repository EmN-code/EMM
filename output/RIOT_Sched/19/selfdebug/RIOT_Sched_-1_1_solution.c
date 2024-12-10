#include "thread_config.h"
#include "thread_arch.h"
#include <stdio.h>
#include <stdlib.h>

// Function for the first thread
void *thread_one(void *arg) {
    while (1) {
        printf("Running Thread One\n");
        // Simulate doing some work
        for (volatile int i = 0; i < 1000000; ++i);
    }
    return NULL;
}

// Function for the second thread
void *thread_two(void *arg) {
    while (1) {
        printf("Running Thread Two\n");
        // Simulate doing some work
        for (volatile int i = 0; i < 1000000; ++i);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one, pid_two;
    char stack_one[THREAD_STACKSIZE_DEFAULT];
    char stack_two[THREAD_STACKSIZE_DEFAULT];

    // Start the first thread
    pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");

    if (pid_one <= KERNEL_PID_UNDEF) {
        printf("Error creating Thread One\n");
        return 1;
    }

    // Start the second thread
    pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");

    if (pid_two <= KERNEL_PID_UNDEF) {
        printf("Error creating Thread Two\n");
        return 1;
    }

    // Change priority of the first thread to a lower priority at runtime
    thread_set_priority(pid_one, THREAD_PRIORITY_MAIN - 2);
    printf("Priority of Thread One changed\n");

    // Change priority of the second thread to a higher priority at runtime
    thread_set_priority(pid_two, THREAD_PRIORITY_MAIN);
    printf("Priority of Thread Two changed\n");

    // Keep the main program running
    while (1) {
        // Simulate doing some work
        for (volatile int i = 0; i < 1000000; ++i);
    }

    return 0;
}