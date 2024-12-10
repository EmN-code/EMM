#include <thread_arch.h>
#include <thread_config.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread_func1(void *arg) {
    printf("Thread 1 running with priority %d\n", thread_get_priority(thread_getpid()));
    // Simulate some work
    for (int i = 0; i < 5; i++) {
        printf("Thread 1 working...\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg) {
    printf("Thread 2 running with priority %d\n", thread_get_priority(thread_getpid()));
    // Simulate some work
    for (int i = 0; i < 5; i++) {
        printf("Thread 2 working...\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    printf("Threads created, now switching priorities.\n");
    xtimer_sleep(2);

    // Change priority at runtime
    sched_set_priority(pid1, THREAD_PRIORITY_MAIN - 2);
    sched_set_priority(pid2, THREAD_PRIORITY_MAIN - 0);

    printf("Priorities changed, thread 1 new priority: %d\n", thread_get_priority(pid1));
    printf("Thread 2 new priority: %d\n", thread_get_priority(pid2));

    // Wait for threads to complete
    thread_join(pid1);
    thread_join(pid2);

    return 0;
}