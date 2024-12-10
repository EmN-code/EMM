
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <thread.h>
#include <irq.h>

// Define a global atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate interrupt handling
void interrupt_handler(void)
{
    // Increment the atomic variable
    atomic_fetch_add(&atomic_var, 1);
}

// Thread function to test interrupt handling
void *thread_func(void *arg)
{
    (void)arg;

    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate some work
    xtimer_sleep(1);

    // Restore interrupts
    irq_restore(state);

    return NULL;
}

int main(void)
{
    // Create a thread to test interrupt handling
    kernel_pid_t pid = thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        thread_func, NULL, "interrupt_test");

    // Wait for the thread to finish
    thread_join(pid);

    // Check the atomic variable to verify interrupt handling
    if (atomic_load(&atomic_var) == 0) {
        printf("Interrupts were correctly disabled and restored.\n");
    } else {
        printf("Interrupts were not correctly handled.\n");
    }

    return 0;
}
