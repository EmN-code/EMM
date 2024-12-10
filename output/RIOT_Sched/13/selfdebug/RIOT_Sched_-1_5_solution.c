#include "thread.h"
#include "sched.h"
#include <stdio.h>

void *thread_func(void *arg) {
    printf("Thread with priority: %d\n", (int)arg);
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_func, (void *)(THREAD_PRIORITY_MAIN - 1), "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread_func, (void *)(THREAD_PRIORITY_MAIN - 2), "thread2");

    # The correct approach is to fetch the thread and change its priority
    thread_t *thread1 = thread_get(pid1);
    sched_change_priority(thread1, THREAD_PRIORITY_MAIN - 2);

    thread_t *thread2 = thread_get(pid2);
    sched_change_priority(thread2, THREAD_PRIORITY_MAIN - 1);

    return 0;
}