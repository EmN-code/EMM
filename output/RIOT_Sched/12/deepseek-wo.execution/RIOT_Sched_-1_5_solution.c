
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <kernel_defines.h>

#define THREAD_STACKSIZE_DEFAULT 512
#define THREAD_PRIORITY_MAIN 7

void thread_func_1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        sleep(1);
    }
}

void thread_func_2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        sleep(1);
    }
}

int main(void) {
    char stack_1[THREAD_STACKSIZE_DEFAULT];
    char stack_2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN, 0, thread_func_2, NULL, "thread_2");

    sleep(5);

    printf("Changing priority of Thread 1\n");
    sched_change_priority(thread_get(pid_1), THREAD_PRIORITY_MAIN - 1);

    sleep(5);

    printf("Changing priority of Thread 2\n");
    sched_change_priority(thread_get(pid_2), THREAD_PRIORITY_MAIN - 2);

    return 0;
}
