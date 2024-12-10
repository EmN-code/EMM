#include "irq_arch.h"
#include <stdio.h>

int main(void) {
    unsigned state;
    volatile int atomic_var = 0; // atomic variable for demonstration purposes

    // Disable interrupts
    state = irq_disable();
    printf("Interrupts disabled. State saved as: %u\n", state);

    // Simulate critical section
    atomic_var = 42; // Example modification
    printf("Atomic variable set to: %d\n", atomic_var);

    // Restore interrupts
    irq_restore(state);
    printf("Interrupts restored.\n");

    // Verify atomic_var value
    if (atomic_var == 42) {
        printf("Atomic variable is consistent: %d\n", atomic_var);
    } else {
        printf("Atomic variable has been altered!\n");
    }

    return 0;
}