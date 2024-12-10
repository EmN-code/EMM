#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h>
#include <kernel_defines.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>

#define THREAD_STACKSIZE_MAIN    1024
#define THREAD_PRIORITY_MAIN     (KERNEL_THREAD_PRIORITY_MAIN - 1)

void *thread_function(void *arg) {
    (void)arg;
    while(1) {
        // Thread does some work here
    }
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];
    kernel_pid_t thread1, thread2;

    // Create two threads
    thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_function, NULL, "thread1");
    thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 1, 0, thread_function, NULL, "thread2");

    // Simulate changing priorities
    // Normally you'd use an available API in your application as RIOT doesn't have direct dynamic priority change
    // Placeholder for dynamic priority change, if available
    // thread_set_priority(thread1, THREAD_PRIORITY_MAIN + 2);

    // Run indefinitely
    while (1) {
        // Main thread work
    }

    return 0;
}