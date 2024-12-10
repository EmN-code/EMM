#include "thread_arch.h"
#include "thread_config.h"
#include <stdio.h>
#include <stdlib.h>

// Thread stack size
#define THREAD_STACKSIZE_MAIN    (1024)

// Thread structure
char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread_func1(void *arg) {
    while (1) {
        printf("Thread 1 running\n");
    }
    return NULL;
}

void *thread_func2(void *arg) {
    while (1) {
        printf("Thread 2 running\n");
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    // Create threads
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN,
                         THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");

    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    // Change thread priorities dynamically
    sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 1);
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN);

    while (1) {}

    return 0;
}