#include "irq_arch.h"
#include <stdio.h>

// Atomic variable for checking interrupt status
volatile bool interrupt_occurred = false;

// Function to simulate interrupt handling
void interrupt_handler(void) {
    // Mark that an interrupt has occurred
    interrupt_occurred = true;
    printf("Interrupt occurred!\n");
}

int main() {
    // Disable interrupts
    irq_disable();
    printf("Interrupts disabled.\n");

    // Simulate an interrupt
    interrupt_handler();

    // Check if interrupt occurred while disabled
    if (interrupt_occurred) {
        printf("Interrupt occurred while disabled!\n");
    } else {
        printf("No interrupt while disabled.\n");
    }

    // Restore interrupts
    irq_restore();
    printf("Interrupts restored.\n");

    // Reset the interrupt flag
    interrupt_occurred = false;

    // Simulate another interrupt
    interrupt_handler();

    // Check if interrupt occurred after restoration
    if (interrupt_occurred) {
        printf("Interrupt occurred after restoration!\n");
    } else {
        printf("No interrupt after restoration.\n");
    }

    return 0;
}