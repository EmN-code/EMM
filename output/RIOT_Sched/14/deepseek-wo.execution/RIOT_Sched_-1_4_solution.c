
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <thread.h>
#include <sched.h>
#include <msg.h>

#define STACK_SIZE 1024
#define THREAD_PRIORITY_HIGH 5
#define THREAD_PRIORITY_LOW 10

static char stack1[STACK_SIZE];
static char stack2[STACK_SIZE];

void *thread_func1(void *arg) {
    printf("Thread 1 started with priority %d\n", sched_get_priority(thread_get_active()));
    while (1) {
        printf("Thread 1 running\n");
        thread_yield();
    }
    return NULL;
}

void *thread_func2(void *arg) {
    printf("Thread 2 started with priority %d\n", sched_get_priority(thread_get_active()));
    while (1) {
        printf("Thread 2 running\n");
        thread_yield();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_LOW, 0, thread_func2, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of Thread 1 to LOW\n");
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_LOW);

    xtimer_sleep(5);

    printf("Changing priority of Thread 2 to HIGH\n");
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_HIGH);

    return 0;
}
