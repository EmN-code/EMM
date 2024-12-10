
#include "thread.h"
#include "sched.h"
#include "msg.h"

#define THREAD_STACKSIZE_DEFAULT (512)
#define DEBUG_EXTRA_STACKSIZE (0)

void thread_func_1(void)
{
    while (1) {
        // Thread 1 logic
        thread_yield();
    }
}

void thread_func_2(void)
{
    while (1) {
        // Thread 2 logic
        thread_yield();
    }
}

int main(void)
{
    char stack_1[THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE];
    char stack_2[THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN - 1,
                                       THREAD_CREATE_STACKTEST, (thread_task_func_t)thread_func_1, NULL, "thread_1");

    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN - 2,
                                       THREAD_CREATE_STACKTEST, (thread_task_func_t)thread_func_2, NULL, "thread_2");

    // Change priority at runtime
    sched_change_priority(pid_1, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(pid_2, THREAD_PRIORITY_MAIN - 1);

    return 0;
}
