#include "thread_config.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "kernel_types.h"

void *thread_one(void *arg) {
    // Thread one code
    while(1) {
        // Perform thread one tasks
    }
    return NULL;
}

void *thread_two(void *arg) {
    // Thread two code
    while(1) {
        // Perform thread two tasks
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid, thread_two_pid;
    char stack_one[THREAD_STACKSIZE_DEFAULT];
    char stack_two[THREAD_STACKSIZE_DEFAULT];
    
    thread_one_pid = thread_create(stack_one, sizeof(stack_one), 
                                    THREAD_PRIORITY_MAIN - 1, 0, 
                                    thread_one, NULL, "thread_one");
    
    thread_two_pid = thread_create(stack_two, sizeof(stack_two), 
                                    THREAD_PRIORITY_MAIN - 2, 0, 
                                    thread_two, NULL, "thread_two");

    // Change priority of thread one to higher than thread two
    thread_set_priority(thread_one_pid, THREAD_PRIORITY_MAIN - 2);
    thread_set_priority(thread_two_pid, THREAD_PRIORITY_MAIN - 1);

    while (1) {
        // Main thread doing its work
    }
    return 0;
}