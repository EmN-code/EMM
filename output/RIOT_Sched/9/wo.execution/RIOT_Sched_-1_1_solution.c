#include "thread_arch.h"
#include "thread_config.h"
#include <stdio.h>

void *thread_func1(void *arg) {
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread_func2(void *arg) {
    printf("Thread 2 is running\n");
    return NULL;
}

int main() {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t pid1, pid2;
    int priority1 = THREAD_PRIORITY_MAIN - 1;
    int priority2 = THREAD_PRIORITY_MAIN - 2;

    // Creating two threads
    pid1 = thread_create(stack1, sizeof(stack1), priority1, 0, thread_func1, NULL, "Thread 1");
    pid2 = thread_create(stack2, sizeof(stack2), priority2, 0, thread_func2, NULL, "Thread 2");

    if (pid1 < 0 || pid2 < 0) {
        printf("Failed to create threads\n");
        return 1;
    }

    printf("Threads created with PIDs %d and %d\n", pid1, pid2);

    // Changing priority
    int new_priority1 = THREAD_PRIORITY_MAIN - 2;
    int new_priority2 = THREAD_PRIORITY_MAIN - 1;

    sched_set_status(thread_get(pid1), thread_getstatus(pid1), new_priority1);
    sched_set_status(thread_get(pid2), thread_getstatus(pid2), new_priority2);

    printf("Changed priorities for thread 1 to %d and thread 2 to %d\n", new_priority1, new_priority2);

    return 0;
}