#include <thread_arch.h>
#include <thread_config.h>
#include <stdio.h>

#define STACKSIZE 1024
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW  (THREAD_PRIORITY_MAIN + 1)

void *thread_func(void *arg) {
    for (int i = 0; i < 5; i++) {
        printf("Thread %ld running\n", (long)arg);
        thread_yield();
    }
    return NULL;
}

int main(void) {
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];
    kernel_pid_t pid1, pid2;

    puts("Main thread now starting other threads\n");

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread_func, (void *)1, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread_func, (void *)2, "thread2");

    printf("Created threads with PIDs: %d, %d\n", pid1, pid2);

    printf("Changing priority of the threads\n");

    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN);

    while (1) {
        thread_yield();
    }
    return 0;
}