#include "thread_config.h"
#include "thread.h"
#include "event/thread.h"
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN)

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

static void *thread1_handler(void *arg) {
    (void)arg;
    printf("Thread 1 is running\n");
    return NULL;
}

static void *thread2_handler(void *arg) {
    (void)arg;
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(stack1, THREAD_STACKSIZE_MAIN,
                                         THREAD_PRIORITY_MAIN,
                                         THREAD_CREATE_STACKTEST,
                                         thread1_handler, NULL, "thread1");

    kernel_pid_t thread2 = thread_create(stack2, THREAD_STACKSIZE_MAIN,
                                         THREAD_PRIORITY_MAIN - 1,
                                         THREAD_CREATE_STACKTEST,
                                         thread2_handler, NULL, "thread2");

    // Change the priority of thread1 to one less than the main thread
    sched_change_priority(thread1, THREAD_PRIORITY_MAIN - 1);

    // Change the priority of thread2 to the main thread priority
    sched_change_priority(thread2, THREAD_PRIORITY_MAIN);

    return 0;
}