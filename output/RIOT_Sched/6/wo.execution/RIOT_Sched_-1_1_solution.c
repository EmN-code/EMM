#include "thread_arch.h"
#include "thread_config.h"
#include <stdio.h>

void *thread_handler_one(void *arg) {
    while (1) {
        printf("Thread One is running\n");
        // Simulate work
        for (volatile int i = 0; i < 1000000; i++);
    }
    return NULL;
}

void *thread_handler_two(void *arg) {
    while (1) {
        printf("Thread Two is running\n");
        // Simulate work
        for (volatile int i = 0; i < 1000000; i++);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid, thread_two_pid;
    char thread_one_stack[THREAD_STACKSIZE_MAIN];
    char thread_two_stack[THREAD_STACKSIZE_MAIN];

    // Create the first thread
    thread_one_pid = thread_create(thread_one_stack, sizeof(thread_one_stack),
                                   THREAD_PRIORITY_MAIN + 1, 0,
                                   thread_handler_one, NULL, "thread_one");

    // Create the second thread
    thread_two_pid = thread_create(thread_two_stack, sizeof(thread_two_stack),
                                   THREAD_PRIORITY_MAIN + 2, 0,
                                   thread_handler_two, NULL, "thread_two");

    // Change the priority of the first thread
    thread_set_priority(thread_one_pid, THREAD_PRIORITY_MAIN + 3);

    return 0;
}