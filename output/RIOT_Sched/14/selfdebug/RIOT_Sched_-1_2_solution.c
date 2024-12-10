#include "thread_arch.h"
#include <stdio.h>
#include <stdlib.h>
#include "kernel.h"
#include "thread.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_LARGE)
#define THREAD_PRIORITY_MAIN     (6)

char thread_one_stack[THREAD_STACKSIZE_MAIN];
char thread_two_stack[THREAD_STACKSIZE_MAIN];

void *thread_one(void *arg) {
    (void)arg;
    printf("Thread One Running\n");
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    printf("Thread Two Running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one;
    kernel_pid_t pid_two;

    pid_one = thread_create(thread_one_stack, sizeof(thread_one_stack),
                            THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                            thread_one, NULL, "Thread One");

    pid_two = thread_create(thread_two_stack, sizeof(thread_two_stack),
                            THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                            thread_two, NULL, "Thread Two");

    printf("Threads created\n");

    // Change priority of the threads at runtime
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN);

    return 0;
}