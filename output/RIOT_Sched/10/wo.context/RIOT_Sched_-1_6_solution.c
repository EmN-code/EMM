#include <thread.h>
#include <stdio.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY1 (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY2 (THREAD_PRIORITY_MAIN - 1)

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

void *thread_func1(void *arg) {
    (void)arg;
    printf("Thread 1 running with priority change capability.\n");
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    printf("Thread 2 running with priority change capability.\n");
    return NULL;
}

int main(void) {
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY1, 0, thread_func1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY2, 0, thread_func2, NULL, "thread2");

    printf("Threads created with initial priorities.\n");

    // Change priority of the threads for demonstration
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN);

    printf("Threads priorities have been changed.\n");

    return 0;
}