
#include <stdio.h>
#include <thread.h>
#include <sched.h>
#include <xtimer.h>
#include <kernel_pid_t.h>

#define THREAD_STACKSIZE_DEFAULT (512)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

void *thread_func_1(void *arg) {
    (void)arg;
    printf("Thread 1 running\n");
    xtimer_sleep(5);
    printf("Thread 1 done\n");
    return NULL;
}

void *thread_func_2(void *arg) {
    (void)arg;
    printf("Thread 2 running\n");
    xtimer_sleep(5);
    printf("Thread 2 done\n");
    return NULL;
}

int main(void) {
    char stack_1[THREAD_STACKSIZE_DEFAULT];
    char stack_2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN, 0, thread_func_2, NULL, "thread_2");

    xtimer_sleep(2);

    printf("Changing priority of Thread 1 to higher\n");
    sched_change_priority(pid_1, THREAD_PRIORITY_MAIN + 1);

    xtimer_sleep(2);

    printf("Changing priority of Thread 2 to higher\n");
    sched_change_priority(pid_2, THREAD_PRIORITY_MAIN + 1);

    return 0;
}
