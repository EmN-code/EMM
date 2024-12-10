
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <kernel_defines.h>

#define STACK_SIZE 512
#define PRIORITY_MAIN 5

void thread_func(void *arg) {
    printf("Thread %s started with priority %d\n", (char *)arg, sched_active_thread_priority());
    while (1) {
        printf("Thread %s running\n", (char *)arg);
        usleep(1000000);
    }
}

int main(void) {
    char stack1[STACK_SIZE];
    char stack2[STACK_SIZE];

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_MAIN, 0, thread_func, "Thread1", "Thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_MAIN, 0, thread_func, "Thread2", "Thread2");

    printf("Main thread started\n");

    usleep(2000000);

    printf("Changing priority of Thread1 to 3\n");
    sched_change_priority(pid1, 3);

    usleep(2000000);

    printf("Changing priority of Thread2 to 7\n");
    sched_change_priority(pid2, 7);

    return 0;
}
