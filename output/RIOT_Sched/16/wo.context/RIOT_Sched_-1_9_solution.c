#include "thread.h"
#include "kernel_defines.h"
#include "board.h"
#include <stdio.h>

#define THREAD_STACKSIZE              (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY               (THREAD_PRIORITY_MAIN - 1)

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread is running.");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid;
    kernel_pid_t thread_two_pid;
    char thread_one_stack[THREAD_STACKSIZE];
    char thread_two_stack[THREAD_STACKSIZE];
    
    thread_one_pid = thread_create(thread_one_stack, sizeof(thread_one_stack),
                                   THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                   thread_handler, NULL, "thread_one");
    printf("Thread 1 PID: %d\n", thread_one_pid);

    thread_two_pid = thread_create(thread_two_stack, sizeof(thread_two_stack),
                                   THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                   thread_handler, NULL, "thread_two");
    printf("Thread 2 PID: %d\n", thread_two_pid);

    // Change the priority of thread_one
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_MAIN);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}