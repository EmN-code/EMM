
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <stdatomic.h>
#include <sched.h>
#include <stdlib.h>
#include <stdbool.h>

#define STACK_SIZE 1024
#define THREAD_PRIORITY_HIGH 5
#define THREAD_PRIORITY_LOW 10

void thread_func_1(void *arg) {
    (void)arg;
    printf("Thread 1 started with priority %d\n", sched_active_thread()->priority);
    sleep(5);
    printf("Thread 1 finished\n");
}

void thread_func_2(void *arg) {
    (void)arg;
    printf("Thread 2 started with priority %d\n", sched_active_thread()->priority);
    sleep(5);
    printf("Thread 2 finished\n");
}

int main(void) {
    char stack_1[STACK_SIZE];
    char stack_2[STACK_SIZE];

    kernel_pid_t pid_1 = thread_create(stack_1, STACK_SIZE, THREAD_PRIORITY_HIGH, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, STACK_SIZE, THREAD_PRIORITY_LOW, 0, thread_func_2, NULL, "thread_2");

    printf("Main thread started\n");

    sleep(2);

    printf("Changing priority of Thread 1 to LOW\n");
    sched_change_priority(thread_get(pid_1), THREAD_PRIORITY_LOW);

    sleep(2);

    printf("Changing priority of Thread 2 to HIGH\n");
    sched_change_priority(thread_get(pid_2), THREAD_PRIORITY_HIGH);

    sleep(10);

    printf("Main thread finished\n");

    return 0;
}
