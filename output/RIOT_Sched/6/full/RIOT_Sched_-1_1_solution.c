#include "thread_arch.h"
#include "thread_config.h"
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SECOND   (THREAD_PRIORITY_MAIN - 2)

void *thread1_handler(void *arg) {
    (void)arg;
    printf("Thread 1: Running with priority %d\n", thread_get_priority(thread_getpid()));
    thread_yield();
    return NULL;
}

void *thread2_handler(void *arg) {
    (void)arg;
    printf("Thread 2: Running with priority %d\n", thread_get_priority(thread_getpid()));
    thread_yield();
    return NULL;
}

int main(void) {
    thread_t thread1, thread2;
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];

    printf("Main: Starting threads\n");

    thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread1_handler, NULL, "thread1");
    thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_SECOND, 0, thread2_handler, NULL, "thread2");

    printf("Main: Changing priorities\n");

    sched_change_priority(thread_getpid(), THREAD_PRIORITY_SECOND);

    char c;
    do {
        c = getchar();
    } while (c != 'q');

    return 0;
}