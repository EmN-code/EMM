#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "sched.h"
#include "event/thread.h"
#include "architecture.h"
#include "xtimer.h"
#include "core.h"

#define THREAD_STACKSIZE_MAIN    (1024)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN)
#define THREAD_PRIORITY_SECOND   (THREAD_PRIORITY_MAIN + 1)

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_one(void *arg) {
    while (1) {
        // Simulate thread activity
        xtimer_sleep(1);
        // Change priority of thread two
        sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN);
    }
    return NULL;
}

void *thread_two(void *arg) {
    while (1) {
        // Simulate thread activity
        xtimer_sleep(2);
        // Change priority of thread one
        sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_SECOND);
    }
    return NULL;
}

int main(void) {
    /* Create two threads */
    thread_one_pid = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN,
                                   THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");
    
    thread_two_pid = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_SECOND,
                                   THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");
    
    // Allow threads to run and switch priorities
    while (1) {
        thread_yield();
    }
    return 0;
}
