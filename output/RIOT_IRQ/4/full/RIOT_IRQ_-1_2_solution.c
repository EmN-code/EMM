#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "irq_arch.h"
#include "irq.h"
#include "mma8x5x_regs.h"

#define IRQ_INITIAL_STATE 0

volatile atomic_bool irq_enabled;

void operations() {
    // Simulate some operations that require interrupts to be disabled.
    printf("Performing operations with interrupts disabled.\n");
}

int main(void) {
    printf("Starting program with interrupt handling.\n");

    unsigned irq_state;

    // Disable interrupts
    irq_state = irq_disable();
    atomic_store(&irq_enabled, false);
    printf("Interrupts disabled, irq_state: %u\n", irq_state);

    // Perform critical operations
    operations();

    // Restore interrupts
    irq_restore(irq_state);
    atomic_store(&irq_enabled, true);
    printf("Interrupts restored.\n");

    return 0;
}