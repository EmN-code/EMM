#include <stdio.h>
#include <thread_config.h>
#include <kernel_defines.h>

void *thread_func(void *arg) {
    printf("Thread running\n");
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t thread_pid1, thread_pid2;

    // Create first thread
    thread_pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 
                                THREAD_CREATE_STACKTEST, thread_func, NULL, "thread1");

    // Create second thread
    thread_pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 
                                THREAD_CREATE_STACKTEST, thread_func, NULL, "thread2");

    // Change priorities of threads at runtime
    sched_change_priority(thread_get(thread_pid1), THREAD_PRIORITY_MAIN);
    sched_change_priority(thread_get(thread_pid2), THREAD_PRIORITY_MAIN - 3);

    return 0;
}