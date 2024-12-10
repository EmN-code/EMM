
#include "thread.h"
#include "sched.h"
#include "stdint.h"
#include "unistd.h"

#define THREAD_STACKSIZE_DEFAULT 512

void thread_func_1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", thread_get_priority(thread_get_active()));
        sleep(1);
    }
}

void thread_func_2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", thread_get_priority(thread_get_active()));
        sleep(1);
    }
}

int main(void) {
    char stack_1[THREAD_STACKSIZE_DEFAULT];
    char stack_2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN + 1, 0, (thread_task_func_t)thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN + 2, 0, (thread_task_func_t)thread_func_2, NULL, "thread_2");

    sleep(5);

    sched_change_priority(thread_get(pid_1), THREAD_PRIORITY_MAIN + 3);
    sched_change_priority(thread_get(pid_2), THREAD_PRIORITY_MAIN + 4);

    while (1) {
        printf("Main thread running at priority %d\n", thread_get_priority(thread_get_active()));
        sleep(1);
    }

    return 0;
}
