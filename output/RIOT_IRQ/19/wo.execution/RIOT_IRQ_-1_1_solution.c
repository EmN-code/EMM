#include <stdio.h>
#include <stdint.h>
#include "irq_arch.h"

int main(void) {
    unsigned irq_status; // Variable to store IRQ status
    
    // Verify if IRQs are enabled initially
    bool interrupts_initial = irq_is_enabled();
    printf("Initial IRQ status: %s\n", interrupts_initial ? "enabled" : "disabled");
    
    // Disable IRQs and store the status
    irq_status = irq_disable();

    // Verify if IRQs are disabled
    bool interrupts_disabled = !irq_is_enabled();
    printf("IRQ status after disable: %s\n", interrupts_disabled ? "disabled" : "still enabled");

    // Restore IRQs to their original state
    irq_restore(irq_status);

    // Check if IRQs are restored
    bool interrupts_restored = irq_is_enabled();
    printf("IRQ status after restore: %s\n", interrupts_restored ? "restored" : "not restored");

    return 0;
}