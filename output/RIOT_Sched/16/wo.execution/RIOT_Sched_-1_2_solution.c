#include "thread_arch.h"
#include "thread.h"
#include "sched.h"
#include "architecture.h"
#include "event/thread.h"

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_LEVELS - 1)
#define THREAD_PRIORITY_IDLE (THREAD_PRIORITY_LEVELS - 1)

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

void *thread_one_handler(void *arg) {
    (void)arg;
    while (1) {
        // Thread work
    }
    return NULL;
}

void *thread_two_handler(void *arg) {
    (void)arg;
    while (1) {
        // Thread work
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one, pid_two;

    pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST | THREAD_CREATE_WOUT_YIELD,
                            thread_one_handler, NULL, "thread_one");

    pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN - 2,
                            THREAD_CREATE_STACKTEST | THREAD_CREATE_WOUT_YIELD,
                            thread_two_handler, NULL, "thread_two");

    // Code to ensure threads start
    
    // Change priority at runtime
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN - 4);

    return 0;
}
