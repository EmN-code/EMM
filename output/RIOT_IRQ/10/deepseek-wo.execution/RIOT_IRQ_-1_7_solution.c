
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <thread.h>
#include <irq.h>

// Define a stack size for the thread
#define THREAD_STACKSIZE (512)

// Define a global atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to be executed by the thread
void *thread_func(void *arg)
{
    (void)arg;

    // Increment the atomic variable
    atomic_fetch_add(&atomic_var, 1);

    return NULL;
}

int main(void)
{
    // Allocate stack for the thread
    char *thread_stack = malloc(THREAD_STACKSIZE);
    if (thread_stack == NULL) {
        puts("Failed to allocate stack for thread");
        return 1;
    }

    // Create the thread
    kernel_pid_t pid = thread_create(thread_stack, THREAD_STACKSIZE,
                                    THREAD_PRIORITY_MAIN - 1, 0,
                                    thread_func, NULL, "worker");

    if (pid <= KERNEL_PID_UNDEF) {
        puts("Failed to create thread");
        free(thread_stack);
        return 1;
    }

    // Wait for the thread to finish
    thread_join(pid);

    // Verify the atomic variable
    if (atomic_load(&atomic_var) == 1) {
        puts("Interrupt handling and atomic operations worked correctly");
    } else {
        puts("Interrupt handling or atomic operations failed");
    }

    // Free the allocated stack
    free(thread_stack);

    return 0;
}
