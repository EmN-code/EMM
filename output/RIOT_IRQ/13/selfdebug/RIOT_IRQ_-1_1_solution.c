#include "irq_arch.h"
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

volatile bool irq_was_disabled = false;
volatile bool irq_was_reenabled = false;

void test_irq_handling(void) {
    // Step 1: Disable interrupts and check status
    unsigned previous_irq_state = irq_disable();
    irq_was_disabled = !irq_is_enabled();

    // Simulating some critical section code
    printf("Interrupts disabled: %s\n", irq_was_disabled ? "Yes" : "No");

    // Step 2: Restore previous interrupt state
    irq_enable(previous_irq_state);
    irq_was_reenabled = irq_is_enabled();

    // Verify and print the result
    printf("Interrupts re-enabled: %s\n", irq_was_reenabled ? "Yes" : "No");
}

int main(void) {
    printf("Testing IRQ handling\n");
    test_irq_handling();
    return 0;
}