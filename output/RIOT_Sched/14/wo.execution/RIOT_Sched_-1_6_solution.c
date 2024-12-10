#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "sched.h"

#define THREAD_STACKSIZE_MAIN THREAD_STACKSIZE_DEFAULT
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

// Declare stack space for two threads
typedef struct {
    char stack[THREAD_STACKSIZE_MAIN];
} thread_stack_t;

thread_stack_t stack_one;
thread_stack_t stack_two;

void *thread_one(void *arg) {
    (void)arg; // Avoid unused variable warning
    while (1) {
        puts("Thread 1 is running");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg; // Avoid unused variable warning
    while (1) {
        puts("Thread 2 is running");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    // Create and start thread one
    kernel_pid_t pid_one = thread_create(stack_one.stack, sizeof(stack_one.stack),
                                         THREAD_PRIORITY_MAIN, 0,
                                         thread_one, NULL, "thread_one");
    
    // Create and start thread two
    kernel_pid_t pid_two = thread_create(stack_two.stack, sizeof(stack_two.stack),
                                         THREAD_PRIORITY_MAIN + 1, 0,
                                         thread_two, NULL, "thread_two");
    
    // Let the main thread sleep for a while
    xtimer_sleep(5);

    // Change priority of thread two
    thread_t *thr = thread_get(pid_two);
    sched_change_priority(thr, THREAD_PRIORITY_MAIN - 1);
    
    return 0;
}