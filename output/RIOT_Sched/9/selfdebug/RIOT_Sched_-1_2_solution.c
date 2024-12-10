#include <thread_config.h>
#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include "sched.h"

#define STACK_SIZE            (THREAD_STACKSIZE_DEFAULT)

static char thread_stack_1[STACK_SIZE];
static char thread_stack_2[STACK_SIZE];

void *thread_handler(void *arg) {
    (void)arg;
    printf("Thread is running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid_1, thread_pid_2;

    /* Create two threads */
    thread_pid_1 = thread_create(thread_stack_1, sizeof(thread_stack_1), 
                                 THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, 
                                 thread_handler, NULL, "thread_1");

    thread_pid_2 = thread_create(thread_stack_2, sizeof(thread_stack_2), 
                                 THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, 
                                 thread_handler, NULL, "thread_2");

    /* Change priorities of the threads at runtime */
    sched_change_priority(thread_get(thread_pid_1), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(thread_pid_2), THREAD_PRIORITY_MAIN - 4);

    printf("Threads created and priorities changed.\n");

    return 0;
}