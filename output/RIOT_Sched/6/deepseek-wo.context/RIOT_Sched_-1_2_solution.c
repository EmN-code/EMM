
#include "thread.h"
#include "sched.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

void thread_func_1(void) {
    while (1) {
        LOG_INFO("Thread 1 running\n");
        thread_yield();
    }
}

void thread_func_2(void) {
    while (1) {
        LOG_INFO("Thread 2 running\n");
        thread_yield();
    }
}

int main(void) {
    log_init();

    char stack_1[THREAD_STACKSIZE_MAIN];
    char stack_2[THREAD_STACKSIZE_MAIN];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN,
                                      THREAD_CREATE_STACKTEST, thread_func_1, NULL, "thread_1");

    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN,
                                      THREAD_CREATE_STACKTEST, thread_func_2, NULL, "thread_2");

    // Change priority of thread 1
    sched_change_priority(thread_get(pid_1), THREAD_PRIORITY_MAIN - 1);

    // Change priority of thread 2
    sched_change_priority(thread_get(pid_2), THREAD_PRIORITY_MAIN - 2);

    return 0;
}
