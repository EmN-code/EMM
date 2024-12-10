#include <stdio.h>
#include "thread.h"

#define STACK_SIZE_THREAD_1 (THREAD_STACKSIZE_DEFAULT)
#define STACK_SIZE_THREAD_2 (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_THREAD_1 (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_THREAD_2 (THREAD_PRIORITY_MAIN - 2)

char stack_thread_1[STACK_SIZE_THREAD_1];
char stack_thread_2[STACK_SIZE_THREAD_2];
kernel_pid_t pid_thread_1;
kernel_pid_t pid_thread_2;

void *thread_handler_1(void *arg) {
    (void)arg; // To avoid `unused parameter` warning
    printf("Thread 1 is running\n");
    while (1) {
        // Thread 1 work here
        thread_yield();
    }
    return NULL;
}

void *thread_handler_2(void *arg) {
    (void)arg; // To avoid `unused parameter` warning
    printf("Thread 2 is running\n");
    while (1) {
        // Thread 2 work here
        thread_yield();
    }
    return NULL;
}

int main(void) {
    printf("Starting RIOT threads on ESP32\n");
    pid_thread_1 = thread_create(stack_thread_1, STACK_SIZE_THREAD_1, PRIORITY_THREAD_1, 0, thread_handler_1, NULL, "thread_1");
    pid_thread_2 = thread_create(stack_thread_2, STACK_SIZE_THREAD_2, PRIORITY_THREAD_2, 0, thread_handler_2, NULL, "thread_2");

    // Simulating priority change at runtime
    printf("Changing priority of threads\n");
    sched_change_priority(thread_get(pid_thread_1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(pid_thread_2), THREAD_PRIORITY_MAIN - 1);

    return 0;
}