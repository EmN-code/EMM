#include "thread.h"
#include "stdio.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD1_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define THREAD2_PRIORITY (THREAD_PRIORITY_MAIN - 2)

char thread1_stack[STACKSIZE];
char thread2_stack[STACKSIZE];

void *thread1_func(void *arg) {
    (void)arg;
    printf("Thread 1 is running with lower priority.\n");
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    printf("Thread 2 is running with higher priority.\n");
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(thread1_stack, sizeof(thread1_stack), THREAD1_PRIORITY, THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(thread2_stack, sizeof(thread2_stack), THREAD2_PRIORITY, THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Simulate change of priority at runtime
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN);

    while(1) {}
    return 0;
}