#include <stdbool.h>  // For bool type
#include <stdint.h>   // For uint32_t type
#include "irq_arch.h"  // For interrupt related functions

volatile bool irq_disabled = false;  // Atomic variable to track IRQ state

void irq_handler(void) {
    if (!irq_disabled) {
        // Simulate interrupt handling work
        // Do some task when interrupt occurs
    }
}

int main(void) {
    // Step 1: Disable interrupts and store previous interrupt state
    unsigned int state = irq_disable();
    irq_disabled = true;  // Set atomic variable to indicate interrupts disabled
    
    // Step 2: Perform critical section operations here
    // Critical section begins
    // 
    //
    // Critical section ends

    // Step 3: Restore interrupts to previous state
    if (irq_disabled) {
        irq_restore(state);
        irq_disabled = false;  // Reset atomic variable to indicate interrupts restored
    }
    
    // Main loop
    while (1) {
        // Could include tasks to run in the main loop
    }

    return 0;
}