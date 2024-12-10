#include <thread_config.h>
#include <kernel_defines.h>
#include <kernel.h>
#include <stdio.h>

void *thread_function(void *arg) {
    printf("Thread is running with argument: %d\n", *(int *)arg);
    return NULL;
}

int main(void) {
    char stack1[KERNEL_CONF_STACKSIZE_DEFAULT];
    char stack2[KERNEL_CONF_STACKSIZE_DEFAULT];
    kernel_pid_t pid1, pid2;
    int arg1 = 1, arg2 = 2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread_function, &arg1,
                         "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread_function, &arg2,
                         "thread2");

    if (pid1 > 0) {
        printf("Thread 1 created with PID: %d\n", pid1);
    } else {
        printf("Failed to create Thread 1\n");
    }

    if (pid2 > 0) {
        printf("Thread 2 created with PID: %d\n", pid2);
    } else {
        printf("Failed to create Thread 2\n");
    }

    // Change priority of the threads
    sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 3);

    printf("Thread priorities changed at runtime\n");

    return 0;
}