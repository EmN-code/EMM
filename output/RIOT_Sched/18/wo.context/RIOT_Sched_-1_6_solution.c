#include "thread.h"
#include "sched.h"
#include "stdio.h"

char thread_stack_one[THREAD_STACKSIZE_MAIN];
char thread_stack_two[THREAD_STACKSIZE_MAIN];

void *thread_handler_one(void *arg) {
    while (1) {
        // Thread one task
        puts("Thread One Running");
    }
    return NULL;
}

void *thread_handler_two(void *arg) {
    while (1) {
        // Thread two task
        puts("Thread Two Running");
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one, pid_two;
    
    pid_one = thread_create(thread_stack_one, sizeof(thread_stack_one),
                            THREAD_PRIORITY_MAIN - 1, 0,
                            thread_handler_one, NULL, "thread_one");
    
    pid_two = thread_create(thread_stack_two, sizeof(thread_stack_two),
                            THREAD_PRIORITY_MAIN - 2, 0,
                            thread_handler_two, NULL, "thread_two");

    // Simulate some delay to allow the threads to initialize
    xtimer_sleep(2);

    // Change the priority of thread two at runtime
    sched_change_priority(pid_two, THREAD_PRIORITY_MAIN - 3);

    return 0;
}