#include <irq_arch.h>

volatile bool interrupt_enabled = true;

// Function to simulate an interrupt disabling and enabling
void toggle_interrupt() {
    unsigned int state;
    
    state = irq_disable(); // Disable interrupts
    interrupt_enabled = false;
    // Simulate some code execution during interrupt disabled
    __asm volatile ("nop");
    __asm volatile ("nop");
    if (!irq_is_enabled()) {
        interrupt_enabled = true;
        irq_restore(state); // Restore interrupt state
    }
}

int main(void) {
    // Initialize the RIOT environment
    printf("Starting Interrupt Handling Example\n");
    
    toggle_interrupt();
    
    if (interrupt_enabled) {
        printf("Interrupt handling implemented correctly.\n");
    } else {
        printf("Error in interrupt handling.\n");
    }
    
    return 0;
}