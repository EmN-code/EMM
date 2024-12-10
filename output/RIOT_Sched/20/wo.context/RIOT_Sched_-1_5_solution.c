#include <thread_config.h> // Include this header to fix implicit declaration error for thread_create
#include <kernel_defines.h>

#include <stdio.h>
#include <stdlib.h>
#include <thread.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 2)

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread_func1(void *arg) {
    (void)arg; // Unused
    printf("Thread 1 running\n");
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg; // Unused
    printf("Thread 2 running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t pids[2];

    // Create two threads
    pids[0] = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_func1, NULL, "thread1");
    pids[1] = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0, thread_func2, NULL, "thread2");

    // Change the priority of the threads
    sched_change_priority(thread_get(pids[0]), THREAD_PRIORITY_MAIN + 1);
    sched_change_priority(thread_get(pids[1]), THREAD_PRIORITY_MAIN + 1);

    return 0;
}