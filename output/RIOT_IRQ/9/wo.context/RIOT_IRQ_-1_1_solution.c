#include <stdbool.h>
#include "irq_arch.h"

// Atomic variable to track interrupt status
typedef volatile bool atomic_bool;

atomic_bool irq_disabled = false;

// Function to disable interrupts and set the atomic variable
void disable_irq(void) {
    irq_disabled = irq_disable();
}

// Function to restore interrupts and verify the atomic variable
void restore_irq(void) {
    if (irq_disabled) {
        irq_restore();
        irq_disabled = false;
    }
}

// Main function
int main(void) {
    // Disable interrupts
    disable_irq();
    
    // Simulate some work while interrupts are disabled
    // ... potentially critical section code

    // Restore interrupts
    restore_irq();

    return 0;
}