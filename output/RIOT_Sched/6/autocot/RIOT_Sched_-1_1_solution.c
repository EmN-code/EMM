#include <thread_config.h>
#include <stdio.h>

// Define stack sizes for the threads
#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN)

char thread_stack_one[THREAD_STACKSIZE_MAIN];
char thread_stack_two[THREAD_STACKSIZE_MAIN];

void *thread_func_one(void *arg) {
    printf("Thread One is running\n");
    return NULL;
}

void *thread_func_two(void *arg) {
    printf("Thread Two is running\n");
    return NULL;
}


int main(void) {
    kernel_pid_t thread_one_pid;
    kernel_pid_t thread_two_pid;

    // Start thread one
    thread_one_pid = thread_create(thread_stack_one, sizeof(thread_stack_one),
                                   THREAD_PRIORITY_MAIN + 1,
                                   THREAD_CREATE_STACKTEST, thread_func_one, NULL, "thread_one");

    // Start thread two
    thread_two_pid = thread_create(thread_stack_two, sizeof(thread_stack_two),
                                   THREAD_PRIORITY_MAIN + 2,
                                   THREAD_CREATE_STACKTEST, thread_func_two, NULL, "thread_two");

    // Change priority of thread one to be lower than thread two
    sched_change_priority(thread_one_pid, THREAD_PRIORITY_MAIN + 3);

    // Change priority of thread two to be higher than thread one
    sched_change_priority(thread_two_pid, THREAD_PRIORITY_MAIN);

    // Let the threads run
    thread_yield();

    return 0;
}
