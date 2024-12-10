#include thread_arch.h
#include thread_config.h
#include stdlib.h
#include "architecture.h"
#include "thread.h"
#include "unistd.h"
#include "event/thread.h"
#include <stddef.h>

static char thread_stack_one[THREAD_STACKSIZE_MAIN];
static char thread_stack_two[THREAD_STACKSIZE_MAIN];

void *thread_function_one(void *arg) {
    (void)arg;
    while (1) {
        // Simulate working
    }
    return NULL;
}

void *thread_function_two(void *arg) {
    (void)arg;
    while (1) {
        // Simulate working
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_thread1 = thread_create(
        thread_stack_one, sizeof(thread_stack_one),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_WOUT_YIELD /* Options */, thread_function_one,
        NULL, "Thread 1");

    kernel_pid_t pid_thread2 = thread_create(
        thread_stack_two, sizeof(thread_stack_two),
        THREAD_PRIORITY_MAIN + 1,
        THREAD_CREATE_WOUT_YIELD /* Options */, thread_function_two,
        NULL, "Thread 2");

    // Change priority of Thread 2 at runtime
    sched_change_priority(thread_get(pid_thread2), THREAD_PRIORITY_MAIN - 2);

    return 0;
}