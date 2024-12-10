#include <irq_arch.h>

int main(void) {
    // Disable interrupts
    unsigned irq_state = irq_disable();
    
    // Critical section of code where interrupts need to be disabled
    // Verify the state of irq (though in real scenarios you would execute your critical code here)
    if (irq_is_disabled()) {
        // Simulate critical operation
        // Atomic operations or sensitive code section could run here
    }
    
    // Restore interrupts to previous state
    irq_restore(irq_state);
    
    // Optionally, check if interrupts are enabled again (for demonstration purposes)
    if (!irq_is_disabled()) {
        // Operation after restoring interrupts
    }
    
    return 0;
}