#include "thread_config.h"
#include "thread_arch.h"

#define STACK_SIZE_THREAD_ONE   (1024)
#define STACK_SIZE_THREAD_TWO   (1024)

char stack_thread_one[STACK_SIZE_THREAD_ONE];
char stack_thread_two[STACK_SIZE_THREAD_TWO];

void *thread_one(void *arg) {
    while(1) {
        // Thread one's operations
    }
    return NULL;
}

void *thread_two(void *arg) {
    while(1) {
        // Thread two's operations
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one, pid_two;

    /* Initialize thread one */
    pid_one = thread_create(stack_thread_one, sizeof(stack_thread_one), 
                            THREAD_PRIORITY_MAIN - 1, 0, thread_one, NULL, "thread_one");

    /* Initialize thread two */
    pid_two = thread_create(stack_thread_two, sizeof(stack_thread_two), 
                            THREAD_PRIORITY_MAIN - 2, 0, thread_two, NULL, "thread_two");

    /* Sleep for some time to allow threads to run */
    xtimer_sleep(2);

    /* Change priority of thread one to a lower priority */
    sched_change_priority(pid_one, THREAD_PRIORITY_MAIN - 3);
    
    return 0;
}