#include "irq_arch.h"
#include <stdio.h>
#include "nimble/nimble_npl.h"

void test_irq_functions(void) {
    unsigned prev_state;
    
    // Disable interrupts
    prev_state = irq_disable();
    printf("Interrupts disabled\n");

    // Check if interrupts are actually disabled
    if (!irq_is_enabled()) {
        printf("Interrupts are confirmed disabled\n");
    }
    
    // Restore the previous interrupt state
    irq_restore(prev_state);
    printf("Interrupts restored to previous state\n");

    // Check if interrupts are enabled again
    if (irq_is_enabled()) {
        printf("Interrupts are enabled after restore\n");
    }
}

int main(void) {
    // Test the interrupt handling functions
    test_irq_functions();
    return 0;
}