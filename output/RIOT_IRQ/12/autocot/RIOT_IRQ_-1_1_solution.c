#include "irq_arch.h"
#include <stdio.h>
#include <stdbool.h>

bool irq_is_enabled(void);
unsigned irq_disable(void);
void irq_restore(unsigned state);

int main() {
    unsigned state;
    bool is_enabled_before;
    bool is_enabled_after;

    // Check the status of the interrupt before disabling
    is_enabled_before = irq_is_enabled();
    printf("IRQ is %s before disabling.\n", is_enabled_before ? "enabled" : "disabled");

    // Disable the interrupts and get the current state
    state = irq_disable();

    // Check the status of the interrupt after disabling
    is_enabled_after = irq_is_enabled();
    printf("IRQ is %s after disabling.\n", is_enabled_after ? "enabled" : "disabled");

    // Restore the interrupts to their previous state
    irq_restore(state);

    // Check the status of the interrupt after restoring
    is_enabled_after = irq_is_enabled();
    printf("IRQ is %s after restoring.\n", is_enabled_after ? "enabled" : "disabled");

    return 0;
}