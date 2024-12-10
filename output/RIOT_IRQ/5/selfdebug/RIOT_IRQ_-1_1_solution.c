#include <irq_arch.h>

volatile unsigned irq_state;

void handle_fake_interrupt() {
    if (irq_is_enabled()) {
        // Disable interrupts
        irq_state = irq_disable();

        // Critical section starts here
        // Simulate some work under interrupt disabled state

        // End of critical section

        // Restore interrupts
        irq_restore(irq_state);
    }
}

int main() {
    // Simulate an interrupt call
    handle_fake_interrupt();
    
    // Add any additional logic needed for testing, or simulate further behavior

    return 0;
}