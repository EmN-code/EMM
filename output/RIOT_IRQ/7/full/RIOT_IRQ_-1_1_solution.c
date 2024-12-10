#include <stdio.h>
#include <stdatomic.h>
#include "irq_arch.h"

int main(void) {
    // Step 1: Initialize an atomic flag
    atomic_int interrupt_flag = 0;
    printf("Initial state of interrupt_flag: %d\n", interrupt_flag);

    // Step 2: Disable interrupts
    unsigned state = irq_disable();

    // Step 3: Modify the atomic variable
    atomic_store(&interrupt_flag, 1);
    printf("Modified state of interrupt_flag (interrupts disabled): %d\n", interrupt_flag);

    // Step 4: Restore interrupts
    irq_restore(state);

    // Verify the result
    printf("Final state of interrupt_flag after restoring interrupts: %d\n", interrupt_flag);

    return 0;
}