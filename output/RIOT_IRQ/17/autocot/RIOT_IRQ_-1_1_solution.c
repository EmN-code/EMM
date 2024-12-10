#include <irq_arch.h>
#include <stdio.h>
#include <stdbool.h>

int main() {
    volatile unsigned int irq_state;
    bool irq_was_enabled;

    printf("Initializing interrupt handling check...\n");

    // Simulate disabling interrupts
    printf("Disabling interrupts...\n");
    irq_state = irq_disable();

    // Verify if IRQ was enabled before
    irq_was_enabled = irq_is_enabled();
    printf("IRQ was %s before disabling.\n", irq_was_enabled ? "enabled" : "disabled");

    // Simulate some critical section code
    printf("Executing critical section...\n");

    // Restore interrupts state
    printf("Restoring interrupts...\n");
    irq_restore(irq_state);

    // Check IRQ status again
    if (irq_is_enabled()) {
        printf("Interrupts restored successfully, IRQ is enabled again.\n");
    } else {
        printf("Failed to restore interrupts, IRQ is still disabled.\n");
    }

    return 0;
}