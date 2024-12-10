#include "thread_config.h"
#include "thread_arch.h"
#include "event/thread.h"
#include "thread.h"
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 5
#define THREAD_PRIORITY_OTHER 4

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread_func1(void *arg) {
    (void) arg; // Suppress unused parameter warning
    printf("Thread 1 running with priority %d\n", THREAD_PRIORITY_MAIN);
    return NULL;
}

void *thread_func2(void *arg) {
    (void) arg; // Suppress unused parameter warning
    printf("Thread 2 running with priority %d\n", THREAD_PRIORITY_OTHER);
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_func1, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_OTHER, 0, thread_func2, NULL, "thread2");

    printf("Two threads have been created.\n");

    thread_set_priority(thread1, THREAD_PRIORITY_OTHER);
    thread_set_priority(thread2, THREAD_PRIORITY_MAIN);

    printf("Thread priorities have been changed.\n");

    return 0;
}