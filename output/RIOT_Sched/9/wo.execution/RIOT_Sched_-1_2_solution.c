// Description of the code solution
// The code creates two threads on an ESP32 using the RIOT OS framework
// The priority of the threads is changed at runtime using the sched_change_priority function
// Required headers are included in descending alphabetical order as specified

// Required imports for the code
#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "sched.h"
#include "event/thread.h"
#include "architecture.h"

#define THREAD_STACKSIZE_MAIN 1024  // Define stack size
#define THREAD_PRIORITY_MAIN 5     // Define main priority

// Thread function prototypes
void *thread_a(void *arg);
void *thread_b(void *arg);

int main(void) {
    char stack_a[THREAD_STACKSIZE_MAIN];  // Stack for thread A
    char stack_b[THREAD_STACKSIZE_MAIN];  // Stack for thread B
    
    kernel_pid_t thread_a_pid, thread_b_pid;
    
    // Create first thread
    thread_a_pid = thread_create(stack_a, THREAD_STACKSIZE_MAIN,
                                THREAD_PRIORITY_MAIN, 0, thread_a, NULL, "thread_a");
    
    // Create second thread
    thread_b_pid = thread_create(stack_b, THREAD_STACKSIZE_MAIN,
                                THREAD_PRIORITY_MAIN, 0, thread_b, NULL, "thread_b");

    // Change priority at runtime
    sched_change_priority(thread_get(thread_a_pid), THREAD_PRIORITY_MAIN - 1);
    sched_change_priority(thread_get(thread_b_pid), THREAD_PRIORITY_MAIN + 1);

    return 0;
}

void *thread_a(void *arg) {
    // Thread A actions
    while (1) {
        // Perform some task
    }
    return NULL;
}

void *thread_b(void *arg) { 
    // Thread B actions
    while (1) {
        // Perform some task
    }
    return NULL;
}