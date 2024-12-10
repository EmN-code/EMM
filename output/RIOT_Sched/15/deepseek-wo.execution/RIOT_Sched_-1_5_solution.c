
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <kernel_defines.h>

#define THREAD_STACKSIZE 2048
#define THREAD_PRIORITY_MAIN 5

typedef void (*thread_task_func_t)(void *arg);

typedef struct {
    char *stack;
    int stacksize;
    uint8_t priority;
    int flags;
    thread_task_func_t task_func;
    void *arg;
    const char *name;
} thread_t;

typedef int kernel_pid_t;

kernel_pid_t thread_create(char *stack, int stacksize, uint8_t priority, int flags, thread_task_func_t task_func, void *arg, const char *name);
void sched_change_priority(kernel_pid_t pid, uint8_t priority);
kernel_pid_t thread_get_active(void);
kernel_pid_t thread_get(kernel_pid_t pid);

void thread_func_1(void *arg) {
    (void)arg;
    printf("Thread 1 running\n");
    while (1) {
        printf("Thread 1 loop\n");
        sleep(1);
    }
}

void thread_func_2(void *arg) {
    (void)arg;
    printf("Thread 2 running\n");
    while (1) {
        printf("Thread 2 loop\n");
        sleep(1);
    }
}

int main(void) {
    char stack1[THREAD_STACKSIZE];
    char stack2[THREAD_STACKSIZE];

    kernel_pid_t pid1 = thread_create(stack1, THREAD_STACKSIZE, THREAD_PRIORITY_MAIN, 0, thread_func_1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, THREAD_STACKSIZE, THREAD_PRIORITY_MAIN, 0, thread_func_2, NULL, "thread2");

    printf("Threads created with PIDs: %d and %d\n", pid1, pid2);

    sleep(5);

    printf("Changing priority of Thread 1 to 3\n");
    sched_change_priority(pid1, 3);

    sleep(5);

    printf("Changing priority of Thread 2 to 7\n");
    sched_change_priority(pid2, 7);

    return 0;
}
