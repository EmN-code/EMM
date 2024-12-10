
#include "thread.h"
#include "kernel_defines.h"
#include "msg.h"

#define THREAD_STACKSIZE_DEFAULT (512)
#define DEBUG_EXTRA_STACKSIZE (0)

void thread_func_1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        thread_yield();
    }
}

void thread_func_2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        thread_yield();
    }
}

int main(void)
{
    char stack_1[THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE];
    char stack_2[THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN - 1,
                                       THREAD_CREATE_STACKTEST, thread_func_1, NULL, "thread_1");

    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN - 2,
                                       THREAD_CREATE_STACKTEST, thread_func_2, NULL, "thread_2");

    // Change priority of thread 1 at runtime
    sched_change_priority(thread_getpid_of(pid_1), THREAD_PRIORITY_MAIN - 3);

    // Change priority of thread 2 at runtime
    sched_change_priority(thread_getpid_of(pid_2), THREAD_PRIORITY_MAIN - 4);

    return 0;
}
