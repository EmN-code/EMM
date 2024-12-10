// Include necessary headers
#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "event/thread.h"
#include "architecture.h"

// Define stack sizes for threads
#define STACKSIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

// Define thread function
void *thread_handler(void *arg) {
    (void)arg; // suppress unused parameter warning
    while(1) {
        // Simulating thread work
    }
    return NULL;
}

int main(void) {
    // Thread stack allocation
    char stack_thread_1[STACKSIZE];
    char stack_thread_2[STACKSIZE];

    // Create threads
    kernel_pid_t thread1_pid = thread_create(stack_thread_1, STACKSIZE,
                                             THREAD_PRIORITY_MAIN - 1,
                                             CREATE_STACKTEST,
                                             thread_handler,
                                             NULL, "thread1");
                                              
    kernel_pid_t thread2_pid = thread_create(stack_thread_2, STACKSIZE,
                                             THREAD_PRIORITY_MAIN - 2,
                                             CREATE_STACKTEST,
                                             thread_handler,
                                             NULL, "thread2");

    // Change thread priorities	hread1_sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);
    thread1_sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 1);

    return 0;
}