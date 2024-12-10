
#include <stdio.h>
#include <stdlib.h>
#include <kernel_defines.h>
#include <sched.h>
#include <thread.h>

#define STACKSIZE 512
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 15

void *thread_func_1(void *arg) {
    (void)arg;
    printf("Thread 1 running\n");
    return NULL;
}

void *thread_func_2(void *arg) {
    (void)arg;
    printf("Thread 2 running\n");
    return NULL;
}

int main(void) {
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread_func_1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread_func_2, NULL, "thread2");

    printf("Threads created: PID1=%d, PID2=%d\n", pid1, pid2);

    // Change priority of thread 1 to low
    sched_change_priority(thread_get(pid1), PRIORITY_LOW);
    printf("Changed priority of thread 1 to low\n");

    // Change priority of thread 2 to high
    sched_change_priority(thread_get(pid2), PRIORITY_HIGH);
    printf("Changed priority of thread 2 to high\n");

    return 0;
}
