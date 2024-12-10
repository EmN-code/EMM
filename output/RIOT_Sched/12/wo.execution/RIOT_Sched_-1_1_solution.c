#include <thread_arch.h>
#include <thread_config.h>
#include <net/protnum.h>

#define THREAD_PRIORITY_MAIN 5
#define THREAD_STACKSIZE_MAIN 1024

// Thread function declarations
void *thread_one(void *arg);
void *thread_two(void *arg);

int main(void) {
    // Initialize threads
    thread_t thread_one_pid;
    char thread_one_stack[THREAD_STACKSIZE_MAIN];

    thread_t thread_two_pid;
    char thread_two_stack[THREAD_STACKSIZE_MAIN];
    
    // Create the threads
    thread_create(thread_one_stack, sizeof(thread_one_stack), THREAD_PRIORITY_MAIN, 0,
                  thread_one, NULL, "thread_one");
    
    thread_create(thread_two_stack, sizeof(thread_two_stack), THREAD_PRIORITY_MAIN, 0,
                  thread_two, NULL, "thread_two");

    // Change priority of the threads at runtime
    sched_set_priority(thread_one_pid, THREAD_PRIORITY_MAIN - 1);
    sched_set_priority(thread_two_pid, THREAD_PRIORITY_MAIN + 1);

    // Keep the main function active
    while (1) {
        
    }

    return 0;
}

// Define thread functions

void *thread_one(void *arg) {
    while (1) {
        // Perform some task
    }
    return NULL;
}

void *thread_two(void *arg) {
    while (1) {
        // Perform some task
    }
    return NULL;
}