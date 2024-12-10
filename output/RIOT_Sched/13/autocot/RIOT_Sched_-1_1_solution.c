#include "thread_config.h"
#include "thread_arch.h"
#include <stdio.h>

void* thread1_function(void* arg) {
    printf("Thread 1 is running\n");
    return NULL;
}

void* thread2_function(void* arg) {
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;
    thread_t thread1, thread2;
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    thread1_pid = thread_create(stack1, sizeof(stack1),
                                THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST,
                                thread1_function, NULL, "thread1");
    printf("Thread 1 PID: %d\n", thread1_pid);

    thread2_pid = thread_create(stack2, sizeof(stack2),
                                THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST,
                                thread2_function, NULL, "thread2");
    printf("Thread 2 PID: %d\n", thread2_pid);

    thread_get(thread1_pid);
    printf("Changing priority of Thread 1\n");
    sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 3);

    thread_get(thread2_pid);
    printf("Changing priority of Thread 2\n");
    sched_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 1);

    return 0;
}