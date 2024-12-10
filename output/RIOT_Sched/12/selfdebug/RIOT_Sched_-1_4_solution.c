#include <thread.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN    THREAD_STACKSIZE_DEFAULT
#define THREAD_PRIORITY_MAIN     8

static char thread_stack_1[THREAD_STACKSIZE_MAIN];
static char thread_stack_2[THREAD_STACKSIZE_MAIN];

void *thread_func(void *arg) {
    (void) arg; // Suppress unused parameter warning
    printf("Thread started with priority %d\n", thread_get_priority(thread_getpid()));
    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid_1;
    kernel_pid_t thread_pid_2;

    // Create first thread
    thread_pid_1 = thread_create(thread_stack_1, sizeof(thread_stack_1),
                                 THREAD_PRIORITY_MAIN + 1,
                                 THREAD_CREATE_STACKTEST,
                                 thread_func, NULL, "thread_1");

    // Create second thread
    thread_pid_2 = thread_create(thread_stack_2, sizeof(thread_stack_2),
                                 THREAD_PRIORITY_MAIN + 2,
                                 THREAD_CREATE_STACKTEST,
                                 thread_func, NULL, "thread_2");

    printf("Thread 1 created with priority %d\n", thread_get_priority(thread_pid_1));
    printf("Thread 2 created with priority %d\n", thread_get_priority(thread_pid_2));

    // Change priority of thread 1
    sched_change_priority(thread_get(thread_pid_1), THREAD_PRIORITY_MAIN + 3);

    printf("Thread 1 priority changed to %d\n", thread_get_priority(thread_pid_1));

    // Change priority of thread 2
    sched_change_priority(thread_get(thread_pid_2), THREAD_PRIORITY_MAIN + 4);

    printf("Thread 2 priority changed to %d\n", thread_get_priority(thread_pid_2));

    return 0;
}