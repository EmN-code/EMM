#include "thread.h"
#include "stdio.h"

void *thread_func(void *arg) {
    (void)arg;  // Suppress unused variable warning
    while(1) {
        printf("This is a child thread running.\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];

    thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_func, NULL, "thread1");

    thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                  THREAD_CREATE_STACKTEST, thread_func, NULL, "thread2");

    // Getting PIDs of threads (this is simulated)
    kernel_pid_t pid1 = thread_getpid();
    kernel_pid_t pid2 = thread_getpid();

    // Change priority of threads (simulated operation)
    int new_priority1 = THREAD_PRIORITY_MAIN - 3;
    int new_priority2 = THREAD_PRIORITY_MAIN - 4;

    sched_change_priority(thread_get(pid1), new_priority1);
    sched_change_priority(thread_get(pid2), new_priority2);

    while(1) {
        printf("This is the main thread running.\n");
        xtimer_sleep(1);
    }
    return 0;
}