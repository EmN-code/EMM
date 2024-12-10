#include "irq_arch.h"

#include <stdio.h>
#include <stdbool.h>

int main() {
    // Initialize atomic integer
    bool irq_enabled = true;

    // Disable interrupts using irq_disable
    irq_enabled = irq_disable();
    printf("Interrupts disabled. Previous state: %s\n", irq_enabled ? "Enabled" : "Disabled");

    // Some critical section code here

    // Restore interrupts to the previous state
    irq_restore(irq_enabled);
    printf("Interrupts restored to previous state: %s\n", irq_enabled ? "Enabled" : "Disabled");

    // Check if interrupts are enabled again
    if (irq_is_enabled()) {
        printf("Interrupts are enabled again as expected.\n");
    } else {
        printf("Interrupts are not enabled as expected.\n");
    }

    return 0;
}