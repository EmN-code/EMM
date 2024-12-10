#include "thread_arch.h"
#include "thread_config.h"
#include <stdio.h>

// Forward declaration of thread functions
void *thread_one(void *arg);
void *thread_two(void *arg);

// Thread stack size
#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

// Thread parameter
kernel_pid_t pid_one;
kernel_pid_t pid_two;
char stack_one[STACKSIZE];
char stack_two[STACKSIZE];

int main(void) {
    // Initialize threads
    pid_one = thread_create(stack_one, sizeof(stack_one),
                            THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST,
                            thread_one, NULL, "thread_one");

    pid_two = thread_create(stack_two, sizeof(stack_two),
                            THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST,
                            thread_two, NULL, "thread_two");

    // Change thread priority of thread_two at runtime
    sched_change_priority(pid_two, THREAD_PRIORITY_MAIN - 2);

    // Main thread spinning
    while (1) {
        thread_yield();
    }

    return 0;
}

void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        puts("This is thread one executing");
        thread_yield();
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        puts("This is thread two executing");
        thread_yield();
    }
    return NULL;
}