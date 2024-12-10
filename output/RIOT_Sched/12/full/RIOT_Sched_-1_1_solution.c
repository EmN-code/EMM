#include "thread_config.h"
#include "kernel_types.h"
#include "xtimer.h"

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread one running with priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread two running with priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t tid_one = thread_create(stack_one, sizeof(stack_one),
                                         THREAD_PRIORITY_MAIN, 0,
                                         thread_one, NULL, "Thread One");

    kernel_pid_t tid_two = thread_create(stack_two, sizeof(stack_two),
                                         THREAD_PRIORITY_MAIN + 1, 0,
                                         thread_two, NULL, "Thread Two");

    printf("Threads created! \n");
    xtimer_sleep(5);

    // Change priority of threads
    sched_change_priority(thread_get(tid_one), THREAD_PRIORITY_MAIN + 1);
    sched_change_priority(thread_get(tid_two), THREAD_PRIORITY_MAIN);

    printf("Thread priorities changed! \n");

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}