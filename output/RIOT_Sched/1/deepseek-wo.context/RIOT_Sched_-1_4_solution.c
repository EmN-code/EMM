
#include "thread.h"
#include "sched.h"
#include "msg.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

void thread_func_1(void) {
    while (1) {
        // Thread 1 logic
        thread_yield();
    }
}

void thread_func_2(void) {
    while (1) {
        // Thread 2 logic
        thread_yield();
    }
}

int main(void) {
    char stack_1[THREAD_STACKSIZE_DEFAULT];
    char stack_2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN - 1,
                                       THREAD_CREATE_WOUT_YIELD, thread_func_1, NULL, "thread_1");

    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN - 2,
                                       THREAD_CREATE_WOUT_YIELD, thread_func_2, NULL, "thread_2");

    // Change priority at runtime
    thread_t *thread_1 = thread_get(pid_1);
    thread_t *thread_2 = thread_get(pid_2);

    sched_change_priority(thread_1, THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_2, THREAD_PRIORITY_MAIN - 4);

    return 0;
}
