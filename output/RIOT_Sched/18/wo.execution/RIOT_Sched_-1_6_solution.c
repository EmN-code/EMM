#include "xtimer.h"
#include "thread.h"
#include "sched.h"
#include "stdio.h"

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 5

char thread_stack1[THREAD_STACKSIZE_MAIN];
char thread_stack2[THREAD_STACKSIZE_MAIN];

void *thread_function1(void *arg) {
    (void)arg;
    printf("Thread 1 running\n");
    while (1) {
        xtimer_sleep(2);
        printf("Thread 1 executing\n");
    }
    return NULL;
}

void *thread_function2(void *arg) {
    (void)arg;
    printf("Thread 2 running\n");
    while (1) {
        xtimer_sleep(3);
        printf("Thread 2 executing\n");
    }
    return NULL;
}

int main(void) {
    kernel_pid_t t1, t2;

    t1 = thread_create(thread_stack1, sizeof(thread_stack1), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread_function1, NULL, "thread1");
    if (t1 < 0) {
        printf("Error creating thread 1\n");
        return 1;
    }

    t2 = thread_create(thread_stack2, sizeof(thread_stack2), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread_function2, NULL, "thread2");
    if (t2 < 0) {
        printf("Error creating thread 2\n");
        return 1;
    }

    // Change priority at runtime
    xtimer_sleep(5);
    printf("Changing priority of threads\n");
    sched_change_priority(thread_get(t1), THREAD_PRIORITY_MAIN + 1);
    sched_change_priority(thread_get(t2), THREAD_PRIORITY_MAIN + 2);

    // Let main thread sleep forever
    thread_sleep();

    return 0;
}