#include "irq_arch.h"

#include <stdio.h>

// Declare an atomic variable
typedef struct {
    volatile unsigned int value;
} atomic_uint_t;

void disable_interrupt_and_verify(atomic_uint_t *atomic_var) {
    // Disable interrupts
    unsigned state = irq_disable();
    
    // Modify the atomic variable to verify changes
    atomic_var->value = 42;

    // Restore interrupts
    irq_restore(state);
}

int main(void) {
    // Initialize atomic variable
    atomic_uint_t atomic_var = {0};

    // Display initial state
    printf("Initial value of atomic_var: %u\n", atomic_var.value);

    // Disable interrupts, modify, and restore
    disable_interrupt_and_verify(&atomic_var);

    // Display modified state
    printf("Value of atomic_var after disabling and restoring interrupts: %u\n", atomic_var.value);

    return 0;
}