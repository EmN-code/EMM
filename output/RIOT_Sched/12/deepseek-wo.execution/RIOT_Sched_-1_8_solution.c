
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <kernel_defines.h>

#define STACKSIZE 1024

void thread_func_1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        sleep(1);
    }
}

void thread_func_2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        sleep(1);
    }
}

int main(void)
{
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_func_1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread_func_2, NULL, "thread2");

    printf("Threads created with PIDs: %d and %d\n", pid1, pid2);

    sleep(5);

    printf("Changing priority of thread 1 to higher\n");
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 3);

    sleep(5);

    printf("Changing priority of thread 2 to higher\n");
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 4);

    return 0;
}
