#include "thread_arch.h"
#include "thread_config.h"

#include <stdio.h>

void *thread_one(void *arg) {
    printf("Thread one is running...\n");
    return NULL;
}

void *thread_two(void *arg) {
    printf("Thread two is running...\n");
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one;
    kernel_pid_t pid_two;

    char stack_one[THREAD_STACKSIZE_MAIN];
    char stack_two[THREAD_STACKSIZE_MAIN];

    pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN - 1, 0,
                            thread_one, NULL, "thread_one");

    if (pid_one <= KERNEL_PID_UNDEF) {
        printf("Failed to start thread one.\n");
    } else {
        printf("Thread one started with PID: %d\n", pid_one);
    }

    pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN - 2, 0,
                            thread_two, NULL, "thread_two");

    if (pid_two <= KERNEL_PID_UNDEF) {
        printf("Failed to start thread two.\n");
    } else {
        printf("Thread two started with PID: %d\n", pid_two);
    }

    // Changing priorities
    printf("Changing priority of threads\n");
    sched_setprio(pid_one, THREAD_PRIORITY_MAIN - 2);
    sched_setprio(pid_two, THREAD_PRIORITY_MAIN - 1);

    return 0;
}