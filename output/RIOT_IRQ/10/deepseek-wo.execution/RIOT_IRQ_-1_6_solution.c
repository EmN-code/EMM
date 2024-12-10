
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <thread.h>
#include <irq.h>

// Global atomic variable to verify interrupt handling
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate some work
void do_work(void) {
    xtimer_sleep(1); // Simulate work by sleeping for 1 second
}

// Function to handle interrupts
void interrupt_handler(void) {
    unsigned state = irq_disable(); // Disable interrupts
    atomic_fetch_add(&atomic_var, 1); // Increment atomic variable
    irq_restore(state); // Restore interrupts
}

// Thread function to periodically trigger interrupts
void *interrupt_thread(void *arg) {
    (void)arg;
    while (1) {
        do_work(); // Simulate some work
        interrupt_handler(); // Trigger interrupt handler
    }
    return NULL;
}

int main(void) {
    // Create a thread to handle interrupts
    thread_t *interrupt_thread_id = thread_create(
        thread_stack,
        sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        interrupt_thread,
        NULL,
        "interrupt_thread"
    );

    // Main loop to periodically check the atomic variable
    while (1) {
        do_work(); // Simulate some work
        printf("Atomic variable value: %d\n", atomic_load(&atomic_var));
        xtimer_sleep(2); // Sleep for 2 seconds before checking again
    }

    // Join the interrupt thread (should never reach here)
    thread_join(interrupt_thread_id);

    return 0;
}
