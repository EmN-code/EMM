#include "thread_config.h"
#include "thread_arch.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN)
#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_MAIN)
#define PRIORITY_CHANGE   5

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread_handler1(void *arg) {
    (void)arg;
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread_handler2(void *arg) {
    (void)arg;
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_handler1, NULL, "thread1");
    if (pid1 < 0) {
        printf("Failed to create thread 1\n");
        return -1;
    }

    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0, thread_handler2, NULL, "thread2");
    if (pid2 < 0) {
        printf("Failed to create thread 2\n");
        return -1;
    }

    printf("Threads created successfully\n");

    sleep(1); // simulate delay before changing priority

    // Change priorities
    if (sched_change_priority(pid1, THREAD_PRIORITY_MAIN + PRIORITY_CHANGE) < 0) {
        printf("Failed to change priority of thread 1\n");
    }

    if (sched_change_priority(pid2, THREAD_PRIORITY_MAIN + PRIORITY_CHANGE) < 0) {
        printf("Failed to change priority of thread 2\n");
    }

    printf("Thread priorities changed successfully\n");

    return 0;
}