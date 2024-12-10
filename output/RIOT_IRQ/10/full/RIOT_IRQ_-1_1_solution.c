#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "irq_arch.h"

volatile bool irq_disabled = false;

void handle_interrupt(void) {
    printf("Interrupt handled.\n");
}

void disable_and_handle_irq() {
    unsigned irq_status = irq_disable();    // Disable interrupts
    irq_disabled = true;    // Update the atomic variable

    // Simulate interrupt handling
    handle_interrupt();

    if (irq_disabled) {
        irq_status = irq_restore(irq_status);    // Restore interrupts
        irq_disabled = false;    // Update the atomic variable
    }
}

int main(void) {
    while (1) {
        // Attempt to disable and restore interrupts
        disable_and_handle_irq();

        // Check if the interrupts were disabled and restored successfully
        if (!irq_disabled) {
            printf("Interrupts restored successfully.\n");
        }
        else {
            printf("Failed to restore interrupts.\n");
        }

        xtimer_sleep(1); // Sleep for 1 second before retry
    }
    return 0;
}
