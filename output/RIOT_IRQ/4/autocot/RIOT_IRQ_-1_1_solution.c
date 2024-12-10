#include <irq_arch.h>
#include <stdbool.h>

volatile bool irq_disabled = false; // Atomic variable to check if IRQs are disabled
volatile bool irq_restored = false; // Atomic variable to check if IRQs are restored

// Function to simulate an interrupt
void interrupt_handler(void) {
    if (irq_is_enabled()) {
        irq_disabled = true; // Set flag if disable was successful
        unsigned state = irq_disable();
        if (!irq_is_enabled()) {
            irq_restored = true; // Set flag if restore was successful
            irq_restore(state);
        }
    }
}

int main(void) {
    // Simulate an interrupt firing
    interrupt_handler();
    
    // Verify that the atomic variables have been set correctly
    if (irq_disabled && irq_restored) {
        // Successful interrupt handling
        return 0;
    }
    
    // Failed interrupt handling
    return 1;
}