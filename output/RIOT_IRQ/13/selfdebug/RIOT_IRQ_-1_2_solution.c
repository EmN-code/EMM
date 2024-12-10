#include "irq_arch.h"

unsigned irq_enable(void); // Declaration of irq_enable

int main(void) {
    // Atomic variable for verifying interrupt status.
    volatile unsigned interrupt_status;
    
    // Disable interrupts and preserve the current state
    interrupt_status = irq_disable();
    
    // Perform operations that require interrupts to be off
    // Here, you can manipulate your atomic variable or perform sensitive tasks
    
    // Restore interrupt state using irq_enable
    if (interrupt_status) {
        irq_enable(); // Only call irq_enable if interrupts were previously enabled
    }

    while (1) {
        // Main loop doing other tasks
    }

    return 0;
}