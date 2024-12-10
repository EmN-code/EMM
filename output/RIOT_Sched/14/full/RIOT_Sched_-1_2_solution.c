#include "thread_config.h"
#include "thread.h"
#include "sched.h"
#include "event/thread.h"

#define STACKSIZE  (THREAD_STACKSIZE_DEFAULT)

void *thread_func_1(void *arg) {
    while (1) {
        // Thread 1 work simulation
    }
    return NULL;
}

void *thread_func_2(void *arg) {
    while (1) {
        // Thread 2 work simulation
    }
    return NULL;
}

int main(void) {
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];
    
    kernel_pid_t thread_pid_1 = thread_create(stack1, sizeof(stack1), 5, 0,
                                              thread_func_1, NULL, "thread_1");

    kernel_pid_t thread_pid_2 = thread_create(stack2, sizeof(stack2), 10, 0,
                                              thread_func_2, NULL, "thread_2");

    // Change priorities at runtime
    irq_disable(); // Disable interrupts before changing priority
    sched_change_priority(thread_get(thread_pid_1), 15);
    sched_change_priority(thread_get(thread_pid_2), 20);
    irq_enable(); // Re-enable interrupts after priorities are changed

    // Simulate an infinite loop to keep main running
    while (1) {
        // Main work simulation
    }

    return 0;
}