
#include "thread.h"
#include "sched.h"
#include "xtimer.h"
#include "kernel_defines.h"
#include "stddef.h"
#include "stdbool.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"

#define THREAD_STACKSIZE_DEFAULT (512)

void thread_func_1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
}

void thread_func_2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
}

int main(void) {
    char stack_1[THREAD_STACKSIZE_DEFAULT];
    char stack_2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN + 1, 0, (thread_task_func_t)thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN + 2, 0, (thread_task_func_t)thread_func_2, NULL, "thread_2");

    xtimer_sleep(5);

    sched_change_priority(thread_get(pid_1), THREAD_PRIORITY_MAIN + 2);
    sched_change_priority(thread_get(pid_2), THREAD_PRIORITY_MAIN + 1);

    return 0;
}
