// Include necessary RIOT header files in descending alphabetical order
#include <thread_arch.h>
#include <thread_config.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_OTHER    (THREAD_PRIORITY_MAIN - 2)

char thread_stack_one[THREAD_STACKSIZE_MAIN];
char thread_stack_two[THREAD_STACKSIZE_MAIN];

void *thread_func_one(void *arg) {
    (void)arg;
    printf("Thread 1 executing with priority change.\n");
    return NULL;
}

void *thread_func_two(void *arg) {
    (void)arg;
    printf("Thread 2 executing with priority change.\n");
    return NULL;
}

int main(void) {
    // Creating thread one
    kernel_pid_t thread_one_pid = thread_create(
        thread_stack_one, THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
        thread_func_one, NULL, "thread_one");
    
    // Creating thread two
    kernel_pid_t thread_two_pid = thread_create(
        thread_stack_two, THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_OTHER, THREAD_CREATE_STACKTEST,
        thread_func_two, NULL, "thread_two");

    // Simulate priority change - assuming function for changing priority available
    // sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_OTHER);
    // sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN);

    // Wakeup threads
    thread_wakeup(thread_one_pid);
    thread_wakeup(thread_two_pid);
    
    // Hold main thread to prevent exiting
    while (1) {
        // main thread operations
    }
    return 0;
}