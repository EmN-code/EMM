#include "thread_config.h"
#include "thread.h"
#include "sched.h"
#include "event/thread.h"
#include <stdio.h>
#include <xtimer.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define HIGH_PRIORITY         (THREAD_PRIORITY_MAIN - 1)
#define LOW_PRIORITY          (THREAD_PRIORITY_MAIN + 1)

char thread_one_stack[THREAD_STACKSIZE_MAIN];
char thread_two_stack[THREAD_STACKSIZE_MAIN];
kernel_pid_t pid_one;
kernel_pid_t pid_two;

void *thread_one(void *arg) {
    while (1) {
        printf("Thread One running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    while (1) {
        printf("Thread Two running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    puts("Starting threads\n");

    pid_one = thread_create(thread_one_stack, sizeof(thread_one_stack),
                            LOW_PRIORITY, 0,
                            thread_one, NULL, "thread_one");

    pid_two = thread_create(thread_two_stack, sizeof(thread_two_stack),
                            LOW_PRIORITY, 0,
                            thread_two, NULL, "thread_two");

    xtimer_sleep(5);

    puts("Changing priorities\n");
    sched_change_priority(thread_get(pid_one), HIGH_PRIORITY);
    sched_change_priority(thread_get(pid_two), HIGH_PRIORITY);

    return 0;
}