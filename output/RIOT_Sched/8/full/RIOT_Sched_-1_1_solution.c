#include "thread_arch.h"
#include "thread_config.h"
#include <stdio.h>
#include <stdlib.h>

static char stack_thread1[THREAD_STACKSIZE_DEFAULT];
static char stack_thread2[THREAD_STACKSIZE_DEFAULT];

void *thread_handler(void *arg) {
    int thread_number = (int)arg;
    printf("Thread %d running with initial priority.\n", thread_number);
    return NULL;
}

int main(void) {
    kernel_pid_t pid_thread1;
    kernel_pid_t pid_thread2;

    printf("Starting threads\n");

    // Create thread 1 with lower priority
    pid_thread1 = thread_create(stack_thread1, sizeof(stack_thread1), 
                                THREAD_PRIORITY_MAIN + 2, THREAD_CREATE_STACKTEST,
                                thread_handler, (void *)1, "thread1");

    // Create thread 2 with higher priority
    pid_thread2 = thread_create(stack_thread2, sizeof(stack_thread2), 
                                THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                thread_handler, (void *)2, "thread2");

    // After some operations or conditions, change the priority of thread 1 to be higher
    sched_change_priority(thread_getpid_of(pid_thread1), THREAD_PRIORITY_MAIN - 2);
    printf("Changed priority of Thread 1 to be higher.\n");

    return 0;
}