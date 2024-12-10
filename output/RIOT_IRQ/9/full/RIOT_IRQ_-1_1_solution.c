#include "thread.h"
#include "irq_arch.h"
#include "xtimer.h"
#include <stdbool.h>

/**
 * This program demonstrates interrupt handling using RIOT's interrupt management.
 * It checks enabling and restoring interrupts using atomic variables.
 */

volatile bool irq_detected = false;

void irq_handler(void) {
    // Simulate an interrupt handler
    irq_detected = true;
}

int main(void) {
    // Disable interrupts and store the state
    unsigned irq_state = irq_disable();
    printf("Interrupts disabled\n");

    // Code that needs to run with interrupts disabled
    if (irq_detected) {
        printf("Interrupt was detected while disabled\n");
        irq_detected = false; // Clear the flag
    }

    // Restore the previous interrupt state
    irq_restore(irq_state);
    printf("Interrupts restored\n");

    // After restoring, simulate an interrupt
    irq_handler();

    // Check if interrupt was detected after restoring
    if (irq_detected) {
        printf("Interrupt was detected after restore\n");
    }
    else {
        printf("No interrupt detected after restore\n");
    }

    return 0;
}