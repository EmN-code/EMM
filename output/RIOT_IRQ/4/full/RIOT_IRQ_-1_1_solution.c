#include "irq_arch.h"
#include <stdio.h>

// Global atomic variable to check interrupt disabling and restoring
static volatile unsigned irq_state;

// Function to simulate some operations
void perform_operations(void) {
    printf("Performing operations in critical section\n");
}

// Main function
int main(void) {
    printf("RTOS Interrupt Handling Example\n");

    // Disable interrupts and save the state
    irq_state = irq_disable();
    printf("Interrupts disabled, state saved: %u\n", irq_state);

    // Perform critical operations
    perform_operations();

    // Restore interrupts to the saved state
    irq_restore(irq_state);
    printf("Interrupts restored to previous state\n");

    return 0;
}