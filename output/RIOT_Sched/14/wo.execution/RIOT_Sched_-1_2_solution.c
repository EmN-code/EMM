#include <thread_config.h>
#include <thread.h>
#include <event/thread.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

void *thread_one(void *arg) {
    (void)arg;
    printf("Thread 1 running with initial priority\n");
    thread_sleep();  // simulate some operation
    printf("Thread 1 priority has been changed at runtime\n");
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    printf("Thread 2 running with initial priority\n");
    thread_sleep();  // simulate some operation
    printf("Thread 2 priority has been changed at runtime\n");
    return NULL;
}

int main(void) {
    printf("Main thread starting\n");

    kernel_pid_t tid_one = thread_create(stack_one, sizeof(stack_one),
                                         THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                                         thread_one, NULL, "thread_one");

    kernel_pid_t tid_two = thread_create(stack_two, sizeof(stack_two),
                                         THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_STACKTEST,
                                         thread_two, NULL, "thread_two");

    // Change thread priority at runtime for demonstration
    sched_change_priority(thread_get(tid_one), THREAD_PRIORITY_MAIN + 2);
    sched_change_priority(thread_get(tid_two), THREAD_PRIORITY_MAIN);

    printf("Threads created and priority changed\n");

    return 0;
}