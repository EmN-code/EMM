#include "irq_arch.h"

// Function to test whether interrupt is currently enabled
void test_interrupts() {
    bool enabled = irq_is_enabled();
    if (enabled) {
        // Disabling interrupts
        unsigned state = irq_disable();

        // Perform some operations

        // Restoring interrupts
        irq_restore(state);
    } else {
        // Handle case where interrupts are not enabled initially
    }
}

int main() {
    // Run the test_interrupts function
    test_interrupts();
    return 0;
}