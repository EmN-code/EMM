#include "thread.h"
#include "sched.h"
#include "event/thread.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

void *thread_a(void *arg) {
    (void)arg;
    while (1) {
        
    }
    return NULL;
}

void *thread_b(void *arg) {
    (void)arg;
    while (1) {
        
    }
    return NULL;
}

int main(void) {
    char stack_a[THREAD_STACKSIZE];
    char stack_b[THREAD_STACKSIZE];

    kernel_pid_t pid_a = thread_create(stack_a, sizeof(stack_a), THREAD_PRIORITY_MAIN - 1, 0, thread_a, NULL, "thread_a");
    kernel_pid_t pid_b = thread_create(stack_b, sizeof(stack_b), THREAD_PRIORITY_MAIN - 2, 0, thread_b, NULL, "thread_b");

    // Function to disable interrupts
    irq_disable();

    // Change the priority of the threads
    sched_change_priority(pid_a, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(pid_b, THREAD_PRIORITY_MAIN - 1);

    // Function to enable interrupts
    irq_enable();

    return 0;
}
