#include <stdint.h>
#include "thread_arch.h"
#include "kernel_defines.h"
#include "kernel.h"

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SECOND  (THREAD_PRIORITY_MAIN - 2)

char stack[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread1_pid, thread2_pid;

void *thread_function(void *arg) {
    (void)arg;
    while(1) {
        // Thread job
    }
    return NULL;
}

int main(void) {
    msg_init_queue(stack, THREAD_STACKSIZE_MAIN);

    // Create two threads
    thread1_pid = thread_create(stack, THREAD_STACKSIZE_MAIN,
                                THREAD_PRIORITY_MAIN, 0,
                                thread_function, NULL, "thread1");

    thread2_pid = thread_create(stack, THREAD_STACKSIZE_MAIN,
                                THREAD_PRIORITY_SECOND, 0,
                                thread_function, NULL, "thread2");
    
    // Run the threads
    thread_wakeup(thread1_pid);
    thread_wakeup(thread2_pid);

    // Change priority at runtime
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_SECOND);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN);

    return 0;
}


