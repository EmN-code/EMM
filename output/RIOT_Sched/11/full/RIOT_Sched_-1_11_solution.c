#include <netinet/in.h>
#include <net/sock/util.h>
#include <stdio.h>
#include "sched.h"

// Define stacks and priorities
char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread_func(void *arg) {
    while (1) {
        // Simulate some work
    }
    return NULL;
}

int main(void) {
    printf("Starting two threads\n");

    // Create threads
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_func, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread_func, NULL, "thread2");

    // Change priorities
    irq_disable();
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 1);
    irq_enable();

    while (1) {
        // Main thread code
    }

    return 0;
}