#include "irq_arch.h"
#include <stdio.h>

volatile bool irq_disabled = false;

void test_irq_functions(void) {
    printf("Disabling IRQs.\n");
    unsigned state = irq_disable();
    irq_disabled = true;
    
    printf("IRQ Disabled: %d\n", irq_disabled);

    // Do some critical operations here that you do not want to be interrupted

    printf("Restoring IRQs.\n");
    irq_restore(state);
    irq_disabled = false;

    printf("IRQ Restored: %d\n", irq_disabled);
}

int main(void) {
    printf("Testing IRQ Disable/Restore\n");
    test_irq_functions();
    return 0;
}