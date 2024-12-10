#include "irq_arch.h"
#include <stdio.h>

// Define a sample atomic variable
volatile atomic_bool irq_enabled = true;

// Function to simulate disabling interrupts
void disable_irq(void) {
    irq_enabled = false;  // Set the atomic variable to false to simulate disabling
    printf("Interrupts disabled\n");
}

// Function to simulate enabling interrupts
void restore_irq(void) {
    irq_enabled = true;  // Set the atomic variable to true to simulate restoring
    printf("Interrupts restored\n");
}

int main(void) {
    // Check if interrupts are initially enabled
    if (irq_enabled) {
        printf("Interrupts are initially enabled\n");
    }

    // Disable interrupts
    disable_irq();

    // Verify interrupts are disabled
    if (!irq_enabled) {
        printf("Verified: Interrupts are disabled\n");
    }

    // Restore interrupts
    restore_irq();

    // Verify interrupts are enabled
    if (irq_enabled) {
        printf("Verified: Interrupts are restored\n");
    }

    return 0;
}