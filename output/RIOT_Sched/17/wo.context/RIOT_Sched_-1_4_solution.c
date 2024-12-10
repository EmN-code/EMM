#include <thread_config.h>
#include <kernel_defines.h>
#include <stdio.h>

void* thread_func(void *arg) {
    printf("Thread %s is running\n", (char *)arg);
    while(1) {
        sched_yield();
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t thread_pid1, thread_pid2;

    thread_pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_func, "1", "thread1");
    printf("Thread 1 created: PID %d\n", thread_pid1);

    thread_pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread_func, "2", "thread2");
    printf("Thread 2 created: PID %d\n", thread_pid2);

    sched_change_priority(thread_get(thread_pid1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread_pid2), THREAD_PRIORITY_MAIN - 1);

    return 0;
}