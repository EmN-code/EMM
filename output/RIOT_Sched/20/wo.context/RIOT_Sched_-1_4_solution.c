#include "thread_config.h"
#include <stdio.h>

void *thread_function(void *arg) {
    while (1) {
        printf("Thread running with argument: %s\n", (char *)arg);
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t thread1_pid, thread2_pid;

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_function, "Thread 1", "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, 0, thread_function, "Thread 2", "thread2");

    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 2);

    while (1) {
        // Main loop
    }

    return 0;
}