#include "thread_arch.h"
#include "thread.h"
#include "thread_config.h"
#include "event/thread.h"
#include "architecture.h"

#include <stdlib.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

char thread_one_stack[THREAD_STACKSIZE_MAIN];
char thread_two_stack[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_one_func(void *arg) {
    (void)arg;
    while (1) {
        // Thread 1 activities
    }
    return NULL;
}

void *thread_two_func(void *arg) {
    (void)arg;
    while (1) {
        // Thread 2 activities
    }
    return NULL;
}

void change_thread_priority(kernel_pid_t pid, uint8_t new_priority) {
    // Custom function to emulate priority changes (requires system support)
    // Placeholder function: change operation implementation specifics will depend on the RTOS capabilities
}

int main(void) {
    puts("Starting two threads...");
    
    thread_one_pid = thread_create(thread_one_stack, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN, 0, thread_one_func, NULL, "thread one");
    thread_two_pid = thread_create(thread_two_stack, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN + 1, 0, thread_two_func, NULL, "thread two");
    
    // Attempt to change priority dynamically
    change_thread_priority(thread_one_pid, THREAD_PRIORITY_MAIN + 1);
    change_thread_priority(thread_two_pid, THREAD_PRIORITY_MAIN);

    while (1) {
        // Main thread activities, possibly monitoring or changing priorities again
    }
    return 0;
}