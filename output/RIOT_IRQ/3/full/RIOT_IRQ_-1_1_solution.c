#include "irq_arch.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    printf("Starting RIOT interrupt handling example on ESP32.\n");

    // Atomic variable to hold interrupt state
    unsigned last_irq_state;

    // Disable interrupts
    last_irq_state = irq_disable();
    printf("Interrupts disabled.\n");

    // Critical section code here
    printf("Simulating critical section.\n");

    // Restore interrupts
    irq_restore(last_irq_state);
    printf("Interrupts restored.\n");

    // Verify if the interrupts were handled correctly
    assert(irq_is_enabled() == true);
    printf("Interrupt handling verified successfully.\n");

    return 0;
}

void irq_handler(void) {
    // Example placeholder for an IRQ handler
    printf("IRQ handler executed.\n");
}