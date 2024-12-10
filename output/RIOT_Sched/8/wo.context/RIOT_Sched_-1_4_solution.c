#include "xtimer.h"
#include "thread.h"
#include <stdio.h>

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_IDLE + 1)
#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)

char stack_1[THREAD_STACKSIZE_MAIN];
char stack_2[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg) {
    (void)arg;
    while(1) {
        printf("Thread %s is running\n", thread_getname(thread_getpid()));
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_1;
    kernel_pid_t pid_2;

    pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN, 0,
                          thread_handler, NULL, "Thread 1");

    pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN + 1, 0,
                          thread_handler, NULL, "Thread 2");

    printf("Threads created with pids %d and %d\n", pid_1, pid_2);

    xtimer_sleep(5); // Let the threads run for a while

    // Change the priority of the threads
    sched_change_priority(thread_get(pid_1), THREAD_PRIORITY_MAIN + 2);
    sched_change_priority(thread_get(pid_2), THREAD_PRIORITY_MAIN + 1);

    printf("Thread priorities have been changed\n");

    xtimer_sleep(5); // Let the threads run with new priorities

    return 0;
}