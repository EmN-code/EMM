#include <thread_config.h>
#include <kernel_defines.h>
#include <stdio.h>
#include "thread.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_NORMAL (THREAD_PRIORITY_MAIN)
#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];
kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread_one(void *arg) {
    (void)arg;
    printf("Thread 1 running\n");
    while (1) {
        // Simulate some workload
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    printf("Thread 2 running\n");
    while (1) {
        // Simulate some workload
    }
    return NULL;
}

int main(void) {
    thread1_pid = thread_create(stack1, THREAD_STACKSIZE, THREAD_PRIORITY_NORMAL, 0,
                                thread_one, NULL, "thread_one");
    thread2_pid = thread_create(stack2, THREAD_STACKSIZE, THREAD_PRIORITY_HIGH, 0,
                                thread_two, NULL, "thread_two");

    // Change the priority of the main thread
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_NORMAL);
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_HIGH);

    // Wake up the threads if they are sleeping
    thread_wakeup(thread1_pid);
    thread_wakeup(thread2_pid);

    return 0;
}