// This code starts two threads on an ESP32 using the RIOT OS and changes priorities at runtime.

#include <ztimer.h> // for ztimer_sleep
#include <stdlib.h> // for malloc and free
#include <stdatomic.h> // for atomic operations
#include <stdbool.h> // for bool type
#include <stdint.h>
#include <stdio.h> // for printf
#include <thread.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_DEFAULT)

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

void *thread_handler1(void *arg) {
    while (1) {
        printf("Thread 1 running\n");
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }
    return NULL;
}

void *thread_handler2(void *arg) {
    while (1) {
        printf("Thread 2 running\n");
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_handler1, NULL, "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 1, 0, thread_handler2, NULL, "thread2");

    // Change priority of thread 1
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN + 1);

    // Change priority of thread 2
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 1);

    return 0;
}