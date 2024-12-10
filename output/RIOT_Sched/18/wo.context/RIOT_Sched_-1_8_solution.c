#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_A (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_B (THREAD_PRIORITY_MAIN - 2)

char stack_one[THREAD_STACKSIZE];
char stack_two[THREAD_STACKSIZE];

void *thread_function_one(void *arg) {
    (void)arg;
    while (1) {
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_function_two(void *arg) {
    (void)arg;
    while (1) {
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one, pid_two;

    pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_A, 0,
                            thread_function_one, NULL, "thread_one");

    pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_B, 0,
                            thread_function_two, NULL, "thread_two");

    // Change priority of thread one
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN);
    // Change priority of thread two
    sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN + 1);

    return 0;
}