#include "thread_arch.h"
#include "thread_config.h"
#include <stdio.h>

// Priority levels for the threads
#define THREAD_ONE_PRIO      (THREAD_PRIORITY_MAIN)
#define THREAD_TWO_PRIO      (THREAD_PRIORITY_MAIN - 1)
#define THREAD_NEW_PRIO      (THREAD_PRIORITY_MAIN + 1)

// Stack size for the threads
#define THREAD_STACKSIZE     (THREAD_STACKSIZE_DEFAULT)

char thread_one_stack[THREAD_STACKSIZE];
char thread_two_stack[THREAD_STACKSIZE];

void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread One Running\n");
        thread_yield();
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread Two Running\n");
        thread_yield();
    }
    return NULL;
}

int main(void) {
    // Initialize thread_one with THREAD_ONE_PRIO
    kernel_pid_t pid_one = thread_create(thread_one_stack, THREAD_STACKSIZE,
                                         THREAD_ONE_PRIO, 0, thread_one, NULL, "thread_one");

    // Initialize thread_two with THREAD_TWO_PRIO
    kernel_pid_t pid_two = thread_create(thread_two_stack, THREAD_STACKSIZE,
                                         THREAD_TWO_PRIO, 0, thread_two, NULL, "thread_two");

    // Simulate some operation during which the threads run
    for (int i = 0; i < 10; ++i) {
        thread_sleep();
    }

    // Change priority of thread_one at runtime
    sched_change_priority(pid_one, THREAD_NEW_PRIO);
    printf("Changed priority of Thread One\n");

    // Simulate again to observe priority change effect
    for (int i = 0; i < 10; ++i) {
        thread_sleep();
    }

    return 0;
}
