#include "xtimer.h"
#include "thread.h"
#include <stdio.h>

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE_MAIN    THREAD_STACKSIZE_MAIN
#define THREAD_PRIORITY_1       (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE_1      THREAD_STACKSIZE_MAIN
#define THREAD_PRIORITY_2       (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE_2      THREAD_STACKSIZE_MAIN

static char stack_1[THREAD_STACKSIZE_1];
static char stack_2[THREAD_STACKSIZE_2];

void *thread_func_1(void *arg) {
    (void)arg;
    puts("Thread 1 is running with priority 1");
    xtimer_sleep(2);
    return NULL;
}

void *thread_func_2(void *arg) {
    (void)arg;
    puts("Thread 2 is running with priority 2");
    xtimer_sleep(2);
    return NULL;
}

int main(void)
{
    kernel_pid_t thread_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_1, 0,
                                          thread_func_1, NULL, "thread_1");

    kernel_pid_t thread_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_2, 0,
                                          thread_func_2, NULL, "thread_2");

    xtimer_sleep(5); // Let threads run for a bit

    puts("Changing priorities...");
    sched_change_priority(thread_1, THREAD_PRIORITY_2);
    sched_change_priority(thread_2, THREAD_PRIORITY_1);

    xtimer_sleep(5); // Allow threads to run with new priorities

    puts("Execution finished");

    return 0;
}