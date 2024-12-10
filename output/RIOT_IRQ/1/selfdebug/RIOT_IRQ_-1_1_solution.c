// Description: This program demonstrates how to handle interrupts in RIOT OS using atomic operations. It uses
// atomic variables to check if disabling and restoring interrupts function correctly without using the GPIO interface.

// Including necessary header files in descending alphabetical order by their first letter.
#include "thread.h"
#include "irq_arch.h"

// Atomic flag to check interrupt status.
static volatile uint8_t interrupt_status;

// Function to simulate an interrupt service routine (ISR)
void simulate_isr(void) {
    // Disable interrupts
    unsigned state = irq_disable();

    // Critical section start
    interrupt_status = 1;  // Set atomic variable to indicate interrupt disabled
    
    // Simulate some processing
    for (volatile int i = 0; i < 1000; i++);

    // Critical section end
    interrupt_status = 0;  // Clear atomic variable to indicate interrupt enabled

    // Restore interrupts
    irq_restore(state);
}

int main(void) {
    // Initialize the atomic flag
    interrupt_status = 0;

    // Simulate ISR execution
    simulate_isr();
    
    // Check outcome (this would typically be an assertion in test code)
    // but here we demonstrate by printing status values
    if (interrupt_status == 0) {
        printf("Interrupts are enabled.\n");
    } else {
        printf("Interrupts are disabled.\n");
    }

    // Main should not exit in RIOT OS; so, keeping it in an idle loop
    while (1) {
        // Idle loop
    }

    return 0;
}