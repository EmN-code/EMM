#include "thread_config.h"
#include "kernel_defines.h"

void *thread_func(void *arg) {
    // Example function for the thread"). This will print a message five times.
    for (int i = 0; i < 5; ++i) {
        printf("Thread %d running...\n", (int)arg);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid_1, thread_pid_2;

    char stack_thread_1[THREAD_STACKSIZE_DEFAULT];
    char stack_thread_2[THREAD_STACKSIZE_DEFAULT];

    thread_pid_1 = thread_create(stack_thread_1, sizeof(stack_thread_1),
                                 THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                                 thread_func, (void *)1, "Thread 1");

    thread_pid_2 = thread_create(stack_thread_2, sizeof(stack_thread_2),
                                 THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                                 thread_func, (void *)2, "Thread 2");

    // Change priorities
    sched_change_priority(thread_get(thread_pid_1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread_pid_2), THREAD_PRIORITY_MAIN - 1);

    return 0;
}