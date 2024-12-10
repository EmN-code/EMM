#include "lwip/pbuf.h"
#include "irq_arch.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>

// Define an atomic variable to check interrupt status
atomic_bool irq_status_checked = ATOMIC_VAR_INIT(false);

// Function to handle interrupt logic
void handle_interrupt(void) {
    // Check and set atomic variable to indicate interrupt handling
    bool expected = false;
    if(atomic_compare_exchange_strong(&irq_status_checked, &expected, true)) {
        printf("Interrupt handled and status checked.\n");
    } else {
        printf("Interrupt already handled.\n");
    }
}

int main() {
    // Disable interrupts
    unsigned state = irq_disable();
    printf("Interrupts disabled.\n");

    // Perform some operations that require interrupts to be off
    handle_interrupt();

    // Restore interrupts to previous state
    irq_restore(state);
    printf("Interrupts restored.\n");

    // Verify interrupt handling
    if (irq_status_checked) {
        printf("Interrupt handling verified successfully.\n");
    } else {
        printf("Failed to check interrupt status correctly.\n");
    }

    return 0;
}