#include "irq_arch.h"
#include <stdio.h>

// Global atomic variable
typedef volatile unsigned irq_status_t;
irq_status_t irq_status;

// Function to simulate an interrupt handler
void interrupt_handler(void) {
    // Perform necessary operations in the interrupt
    printf("Interrupt triggered and handled.\n");
}

// Main function
int main(void) {
    // Check if IRQ is enabled initially
    if (irq_is_enabled()) {
        printf("IRQ initially enabled. Disabling IRQ now.\n");
    } else {
        printf("IRQ initially disabled.\n");
    }

    // Disable IRQ and store the status
    irq_status = irq_disable();
    printf("IRQ disabled. Status saved.\n");

    // Perform main logic
    printf("Performing main operations without IRQ interference.\n");
    
    // Simulate an interrupt
    interrupt_handler();

    // Restore IRQ
    irq_restore(irq_status);
    printf("IRQ restored to previous state.\n");

    // Final message
    printf("Program finished.\n");

    return 0;
}