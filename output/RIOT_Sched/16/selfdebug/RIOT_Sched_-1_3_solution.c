#include <thread_arch.h>
#include <kernel_defines.h>
#include <stdio.h>

void *thread_func(void *arg) {
    printf("Thread %d running\n", thread_getpid());
    sched_yield();
    printf("Thread %d completed\n", thread_getpid());
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t thread1_pid = thread_create(stack1, sizeof(stack1),
                                            THREAD_PRIORITY_MAIN - 2,
                                            THREAD_CREATE_STACKTEST,
                                            thread_func, NULL, "thread1");

    kernel_pid_t thread2_pid = thread_create(stack2, sizeof(stack2),
                                            THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST,
                                            thread_func, NULL, "thread2");

    thread_t *thread1 = thread_get(thread1_pid);
    thread_t *thread2 = thread_get(thread2_pid);

    sched_change_priority(thread1, THREAD_PRIORITY_MAIN);
    sched_change_priority(thread2, THREAD_PRIORITY_MAIN - 1);

    return 0;
}