#include "thread_config.h"
#include "kernel_defines.h"

#define MY_THREAD_STACKSIZE  (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE)
#define MY_SECOND_THREAD_STACKSIZE  (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE)

char thread_stack_one[MY_THREAD_STACKSIZE];
char thread_stack_two[MY_SECOND_THREAD_STACKSIZE];

void *thread_one_func(void *arg) {
    (void)arg;
    while (1) {
        // Perform tasks for thread one
    }
    return NULL;
}

void *thread_two_func(void *arg) {
    (void)arg;
    while (1) {
        // Perform tasks for thread two
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid = thread_create(
        thread_stack_one, MY_THREAD_STACKSIZE,
        THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
        thread_one_func, NULL, "thread_one");
    
    kernel_pid_t thread_two_pid = thread_create(
        thread_stack_two, MY_SECOND_THREAD_STACKSIZE,
        THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
        thread_two_func, NULL, "thread_two");

    // Change priority of thread one
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_MAIN - 3);

    // Change priority of thread two
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN - 4);

    return 0;
}