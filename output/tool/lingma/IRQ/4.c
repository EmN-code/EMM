#include "xtimer.h"
#include "irq.h"
#include "thread.h"

// Atomic variable to check if the interrupt handler has been called
volatile bool interrupt_triggered = false;

// Interrupt handler function
void interrupt_handler(void *arg) {
    (void)arg;
    // Set the atomic variable to true
    atomic_set(&interrupt_triggered, true);
    // Print a message indicating the interrupt has been triggered
    printf("Interrupt triggered!\n");
}

// Function to simulate an interrupt
void trigger_interrupt(void) {
    // Simulate an interrupt by calling the handler directly
    interrupt_handler(NULL);
}

int main(void) {
    // Initialize the system
    printf("Starting interrupt handling example...\n");

    // Disable interrupts
    irq_disable();

    // Check if interrupts are disabled
    if (irq_is_disabled()) {
        printf("Interrupts are disabled.\n");
    } else {
        printf("Failed to disable interrupts.\n");
        return 1;
    }

    // Simulate an interrupt
    trigger_interrupt();

    // Check if the interrupt handler was called
    if (atomic_get(&interrupt_triggered)) {
        printf("Interrupt handler was called while interrupts were disabled.\n");
    } else {
        printf("Interrupt handler was not called as expected.\n");
    }

    // Restore interrupts
    irq_restore(irq_disable());

    // Check if interrupts are enabled
    if (!irq_is_disabled()) {
        printf("Interrupts are enabled.\n");
    } else {
        printf("Failed to enable interrupts.\n");
        return 1;
    }

    // Simulate an interrupt again
    trigger_interrupt();

    // Check if the interrupt handler was called
    if (atomic_get(&interrupt_triggered)) {
        printf("Interrupt handler was called while interrupts were enabled.\n");
    } else {
        printf("Interrupt handler was not called as expected.\n");
    }

    // Main loop
    while (1) {
        xtimer_sleep(1); // Sleep for 1 second
    }

    return 0;
}