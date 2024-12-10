#include <thread_config.h>
#include <kernel_defines.h>
#include <stdio.h>
#include "thread.h"

#define THREAD_STACKSIZE 1024
#define THREAD_PRIORITY_MAIN 255
#define THREAD_PRIORITY_1 (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_2 (THREAD_PRIORITY_MAIN - 2)

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

void *thread_func(void *arg) {
    (void)arg;
    printf("Thread running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    // Create first thread
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_1, THREAD_CREATE_STACKTEST, thread_func, NULL, "thread1");
    printf("Thread 1 created with PID %d\n", pid1);

    // Create second thread
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_2, THREAD_CREATE_STACKTEST, thread_func, NULL, "thread2");
    printf("Thread 2 created with PID %d\n", pid2);

    // Change priority of the threads
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_2);
    printf("Thread 1 priority changed\n");

    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_1);
    printf("Thread 2 priority changed\n");

    return 0;
}