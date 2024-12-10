#include <thread.h>  // For thread_zombify()
#include <irq_arch.h> // For irq_disable(), irq_restore(), and irq_is_enabled()

volatile unsigned int interrupt_flag = 0; // Atomic variable for interrupt handling

// Interrupt handler function
void interrupt_handler(void) {
    if (irq_is_enabled()) {
        // If interrupts are enabled, disable them
        unsigned int state = irq_disable();
        // Perform critical work here
        
        // Restore interrupts
        irq_restore(state);
    }
    else {
        // If already disabled, do different processing or logging
    }
}

int main(void) {
    // Simulate the main program loop
    while (1) {
        // Check or perform some conditions to invoke the interrupt handler
        if (interrupt_flag) {
            interrupt_handler();
            // Reset the flag for next interrupt
            interrupt_flag = 0;
        }
        thread_zombify(); // Yield the CPU
    }
    return 0;
}