#include "irq_arch.h"
#include <stdio.h>
#include <stdatomic.h>

// Atomic variable to track interrupt state
atomic_int irq_state;

void change_irq_state(void) {
    // Disable interrupts – atomic section
    unsigned state = irq_disable();
    atomic_store(&irq_state, 1);  // Indicate IRQs are disabled

    // Pretend to do some critical work here

    // Restore interrupts
    irq_restore(state);
    atomic_store(&irq_state, 0);  // Indicate IRQs are enabled
}

int main(void) {
    // Initialize the atomic variable
    atomic_init(&irq_state, 0);
    printf("Initial IRQ state: %d\n", irq_state);

    // Change IRQ state
    change_irq_state();

    // Print the final state
    printf("Final IRQ state after operations: %d\n", irq_state);

    return 0;
}