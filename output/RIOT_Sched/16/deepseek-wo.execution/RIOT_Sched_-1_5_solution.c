
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>

#define STACKSIZE 1024

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
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_MAIN - 1, 0, thread_func_1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_MAIN - 2, 0, thread_func_2, NULL, "thread2");

    sleep(5);

    sched_change_priority(pid1, PRIORITY_MAIN - 2);
    sched_change_priority(pid2, PRIORITY_MAIN - 1);

    printf("Priorities changed\n");

    return 0;
}
