#include "thread_config.h"
#include "thread_arch.h"

#include <stdio.h>
#include <stdlib.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN)

void *thread1(void *arg) {
    while (1) {
        printf("Running thread 1 with priority: %d\n", (int)thread_get_priority(thread_getpid()));
        thread_yield_higher();
    }
    return NULL;
}

void *thread2(void *arg) {
    while (1) {
        printf("Running thread 2 with priority: %d\n", (int)thread_get_priority(thread_getpid()));
        thread_yield_higher();
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];

    thread_create(stack1, THREAD_STACKSIZE_MAIN,
                  THREAD_PRIORITY_MAIN, 0,
                  thread1, NULL, "thread1");

    thread_create(stack2, THREAD_STACKSIZE_MAIN,
                  THREAD_PRIORITY_MAIN, 0,
                  thread2, NULL, "thread2");

    /* Change the priority of the threads at runtime */
    kernel_pid_t pid1 = thread_getpid_of(thread_get(kernel_pid_t(1)));
    kernel_pid_t pid2 = thread_getpid_of(thread_get(kernel_pid_t(2)));

    thread_set_priority(pid1, THREAD_PRIORITY_MAIN - 1);
    thread_set_priority(pid2, THREAD_PRIORITY_MAIN - 2);

    while (1) {
        thread_yield_higher();
    }
    return 0;
}