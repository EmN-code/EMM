#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "sched.h"
#include "event/thread.h"
#include "architecture.h"

#define STACKSIZE_MAIN THREAD_STACKSIZE_MAIN
#define STACKSIZE_THREAD (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

char thread_stack[STACKSIZE_THREAD];

void *thread_entry(void *arg) {
    (void)arg;
    while (1) {
        // Do some work here
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid;
    kernel_pid_t main_thread_pid = thread_get_active();
    
    // Create a new thread
    pid = thread_create(thread_stack, STACKSIZE_THREAD, THREAD_PRIORITY_MAIN,
                        THREAD_CREATE_STACKTEST, thread_entry, NULL, "thread_1");
    if (pid <= KERNEL_PID_UNDEF) {
        // Handle thread creation error
        return 1;
    }

    // Simulate a change in priority
    uint8_t new_priority = THREAD_PRIORITY_MAIN + 1;
    sched_change_priority(pid, new_priority);

    // Also change the main thread's priority
    sched_change_priority(main_thread_pid, new_priority);

    while (1) {
        // Main loop
    }

    return 0;
}