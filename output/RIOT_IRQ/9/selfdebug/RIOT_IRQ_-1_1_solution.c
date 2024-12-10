#include <irq_arch.h>
#include <stdio.h>

// Global atomic variable
volatile unsigned irq_state;

// Function to check if interrupt is enabled or disabled
bool check_irq_status(void) {
    return irq_is_enabled();
}

// Main function
int main(void) {
    printf("Starting Interrupt Handling Test...\n");
    
    // Initial IRQ status
    bool is_enabled = check_irq_status();
    printf("Initial IRQ Enabled: %s\n", is_enabled ? "Yes" : "No");

    // Disable interrupts
    irq_state = irq_disable();
    is_enabled = check_irq_status();
    printf("IRQ Enabled after disable: %s\n", is_enabled ? "Yes" : "No");

    // Restore interrupts
    irq_restore(irq_state);
    is_enabled = check_irq_status();
    printf("IRQ Enabled after restore: %s\n", is_enabled ? "Yes" : "No");

    return 0;
}