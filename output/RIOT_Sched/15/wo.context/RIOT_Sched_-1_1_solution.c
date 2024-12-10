#include <thread_arch.h>
#include <thread_config.h>
#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN)

static char thread_stack_one[THREAD_STACKSIZE_MAIN];
static char thread_stack_two[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread one running at priority %d\n", thread_get_priority(thread_one_pid));
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread two running at priority %d\n", thread_get_priority(thread_two_pid));
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    printf("Starting RIOT threads for priority change example\n");

    thread_one_pid = thread_create(thread_stack_one, sizeof(thread_stack_one),
                             THREAD_PRIORITY_MAIN + 1,
                             THREAD_CREATE_STACKTEST,
                             thread_one, NULL, "thread_one");

    thread_two_pid = thread_create(thread_stack_two, sizeof(thread_stack_two),
                             THREAD_PRIORITY_MAIN + 1,
                             THREAD_CREATE_STACKTEST,
                             thread_two, NULL, "thread_two");

    xtimer_sleep(3);

    printf("Changing priorities at runtime\n");

    int prio = thread_get_priority(thread_one_pid);
    printf("Current priority of thread one: %d\n", prio);

    prio = thread_get_priority(thread_two_pid);
    printf("Current priority of thread two: %d\n", prio);

    thread_set_priority(thread_one_pid, THREAD_PRIORITY_MAIN - 1);
    thread_set_priority(thread_two_pid, THREAD_PRIORITY_MAIN + 2);

    printf("Priorities changed.\n");

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}