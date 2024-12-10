#include "thread_config.h"
#include "thread_arch.h"
#include "thread.h"
#include "sched.h"
#include "event/thread.h"
#include "architecture.h"

#include <stdio.h>
#include <stdlib.h>

#define THREAD_STACKSIZE_DEFAULT 512

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_handler(void *arg) {
    (void)arg;
    
    while (1) {
        // Simulate work with a simple print statement
        printf("Thread running with priority: %d\n", thread_getpid());
        // Suspend thread execution
        thread_yield();
    }
    
    return NULL;
}

int main(void) {
    kernel_pid_t thread1;
    kernel_pid_t thread2;
    
    // Create two threads
    thread1 = thread_create(stack1, sizeof(stack1), 5, 0, thread_handler, NULL, "thread 1");
    thread2 = thread_create(stack2, sizeof(stack2), 7, 0, thread_handler, NULL, "thread 2");
    
    // Change priority of the threads at runtime
    sched_change_priority(thread_get(thread1), 3);
    sched_change_priority(thread_get(thread2), 6);

    for (;;) {
        // Main loop could do other things, we just wait here
    }

    return 0;
}