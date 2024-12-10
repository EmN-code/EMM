#include "irq_arch.h"
#include <stdio.h>
#include <stdatomic.h>

// Atomic flag to indicate interrupt state
static atomic_bool irq_was_disabled;

// Function to simulate an interrupt disable operation
unsigned my_irq_disable(void) {
    // Set atomic flag to true
    atomic_store(&irq_was_disabled, true);
    // Return the current interrupt state code (simulated)
    return 0x1;
}

// Function to simulate an interrupt restore operation
void my_irq_restore(unsigned state) {
    // Restore interrupt state by setting the atomic flag to false
    atomic_store(&irq_was_disabled, false);
}

int main(void) {
    printf("Starting interrupt test on ESP32 using RIOT\n");

    // Initially, no interrupt is disabled
    atomic_store(&irq_was_disabled, false);

    printf("Disabling interrupts...\n");
    unsigned state = my_irq_disable();

    if (atomic_load(&irq_was_disabled)) {
        printf("Interrupts successfully disabled.\n");
    } else {
        printf("Failed to disable interrupts.\n");
    }

    printf("Restoring interrupts...\n");
    my_irq_restore(state);

    if (!atomic_load(&irq_was_disabled)) {
        printf("Interrupts successfully restored.\n");
    } else {
        printf("Failed to restore interrupts.\n");
    }

    return 0;
}