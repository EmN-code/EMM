#include "thread_arch.h"
#include "thread_config.h"
#include <stdio.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY  (THREAD_PRIORITY_MAIN - 1)
#define NEW_PRIORITY (THREAD_PRIORITY_MAIN - 2)

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread_handler(void *args) {
    printf("Thread executing with priority %d\n", (int)args);
    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid_1, thread_pid_2;

    thread_pid_1 = thread_create(stack1, sizeof(stack1), PRIORITY, 0, thread_handler, (void *)PRIORITY, "thread_1");
    if (thread_pid_1 <= KERNEL_PID_UNDEF) {
        printf("Error creating thread 1\n");
        return 1;
    }

    thread_pid_2 = thread_create(stack2, sizeof(stack2), PRIORITY, 0, thread_handler, (void *)PRIORITY, "thread_2");
    if (thread_pid_2 <= KERNEL_PID_UNDEF) {
        printf("Error creating thread 2\n");
        return 1;
    }

    printf("Threads started\n");

    // Change priority of thread 1
    sched_change_priority(thread_pid_1, NEW_PRIORITY);
    printf("Changed priority of thread 1\n");

    return 0;
}