#include <thread_arch.h>
#include <thread_config.h>
#include "thread.h"
#include "event/thread.h"
#include <xtimer.h>

#define THREAD_STACKSIZE_MAIN (2048)
#define PRIORITY_ONE (THREAD_PRIORITY_MAIN - 1)

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread One is running");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread Two is running");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    puts("Starting RIOT application\n");

    thread_one_pid = thread_create(
        stack_one, sizeof(stack_one), PRIORITY_ONE,
        THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");

    thread_two_pid = thread_create(
        stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");

    xtimer_sleep(5);

    puts("Changing priorities\n");
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_MAIN + 1);
    sched_change_priority(thread_get(thread_two_pid), PRIORITY_ONE);

    while (1) {
     xtimer_sleep(1);   // Keep the main thread running
    }

    return 0;
}
