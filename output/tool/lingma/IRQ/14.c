#include <stdio.h>
#include <atomic.h>
#include "xtimer.h"
#include "irq.h"

// Atomic variable to check if the interrupt handler has been called
static atomic_t interrupt_triggered = ATOMIC_INIT(0);

// Interrupt handler function
void my_interrupt_handler(void *arg) {
    (void)arg; // Unused argument
    printf("Interrupt triggered!\n");
    atomic_set(&interrupt_triggered, 1);
}

// Function to simulate an interrupt source
void trigger_interrupt(void) {
    // Simulate an interrupt by calling the handler directly
    my_interrupt_handler(NULL);
}

int main(void) {
    // Disable interrupts
    irq_disable();

    // Simulate triggering an interrupt
    trigger_interrupt();

    // Check if the interrupt handler was called while interrupts were disabled
    if (atomic_get(&interrupt_triggered) == 1) {
        printf("Interrupt handler was called while interrupts were disabled!\n");
    } else {
        printf("Interrupt handler was not called while interrupts were disabled.\n");
    }

    // Restore interrupts
    irq_restore();

    // Simulate triggering an interrupt again
    trigger_interrupt();

    // Check if the interrupt handler was called after restoring interrupts
    if (atomic_get(&interrupt_triggered) == 1) {
        printf("Interrupt handler was called after restoring interrupts.\n");
    } else {
        printf("Interrupt handler was not called after restoring interrupts.\n");
    }

    // Main loop
    while (1) {
        xtimer_sleep(1); // Sleep for 1 second
    }

    return 0;
}