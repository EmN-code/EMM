#include "irq_arch.h"

#include <stdio.h>

#include <stdatomic.h>

atomic_bool irq_status_restored;

int main(void) {
    unsigned state;

    // Disable interrupts and store the current state
    state = irq_disable();
    printf("Interrupts disabled\n");

    // Modify the atomic variable
    atomic_store(&irq_status_restored, false);

    // Restore interrupts
    irq_restore(state);
    printf("Interrupts restored\n");

    // Set atomic variable to true after restoring
    atomic_store(&irq_status_restored, true);

    // Print the status
    if (atomic_load(&irq_status_restored))
        printf("IRQs restored successfully and verified with atomic variable.\n");
    else
        printf("Failed to verify IRQ restoration.\n");

    return 0;
}