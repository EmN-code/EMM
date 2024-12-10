#include <irq_arch.h>

// Function to enable an interrupt and return its status
bool enable_irq(void) {
    unsigned state = irq_disable();
    if (!irq_is_enabled()) {
        irq_restore(state);
    }
    return irq_is_enabled();
}

// Function to disable an interrupt and return its status
bool disable_irq(void) {
    irq_disable();
    return !irq_is_enabled();
}

// Main function to test the enabling and disabling of interrupts
int main(void) {
    // Start with interrupts enabled
    if (disable_irq()) {
        // Interrupts disabled successfully
        if (enable_irq()) {
            // Interrupts enabled successfully
            return 0; // Success
        }
    }
    return 1; // Failure
}