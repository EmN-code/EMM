#include <stdio.h>
#include <atomic.h>
#include "xtimer.h"
#include "irq.h"

// Define an atomic variable to track the interrupt status
static atomic_t interrupt_triggered;

// Interrupt handler function
void handle_interrupt(void *arg) {
    (void)arg;
    // Atomically set the interrupt_triggered flag
    atomic_set(&interrupt_triggered, 1);
    printf("Interrupt triggered!\n");
}

// Function to simulate an interrupt
void trigger_interrupt(void) {
    // Simulate an interrupt by calling the handler directly
    handle_interrupt(NULL);
}

int main(void) {
    // Initialize the atomic variable
    atomic_init(&interrupt_triggered, 0);

    // Enable global interrupts
    irq_enable();

    // Simulate triggering an interrupt
    trigger_interrupt();

    // Check the atomic variable to see if the interrupt was handled
    if (atomic_read(&interrupt_triggered) == 1) {
        printf("Interrupt handling verified.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    // Disable global interrupts
    irq_disable();

    // Restore the atomic variable to its initial state
    atomic_init(&interrupt_triggered, 0);

    // Re-enable global interrupts
    irq_enable();

    // Simulate triggering another interrupt
    trigger_interrupt();

    // Check the atomic variable again
    if (atomic_read(&interrupt_triggered) == 1) {
        printf("Interrupt handling verified after re-enabling.\n");
    } else {
        printf("Interrupt handling failed after re-enabling.\n");
    }

    return 0;
}