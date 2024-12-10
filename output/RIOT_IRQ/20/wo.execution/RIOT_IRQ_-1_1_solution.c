#include "lwip/pbuf.h"
#include "irq_arch.h"
#include <stdio.h> // For standard input and output functions

static volatile unsigned irq_status;

void test_interrupt_handling(void) {
    // Disabling interrupts
    irq_status = irq_disable();
    printf("Interrupts disabled, status saved.\n");

    // Perform operations with interrupts disabled.
    // ... (e.g., modify shared variables, etc.)

    // Check if interrupts are enabled
    if (irq_is_enabled()) {
        printf("Error: Interrupts are still enabled.\n");
    } else {
        printf("Interrupts are currently disabled.\n");
    }

    // Restoring interrupts
    irq_restore(irq_status);
    printf("Interrupts restored to previous state.\n");

    // Verify restoring functionality
    if (irq_is_enabled()) {
        printf("Interrupts are properly restored and enabled.\n");
    } else {
        printf("Error: Interrupts are not restored correctly.");
    }
}

int main(void) {
    printf("Starting interrupt handling test on ESP32 with RIOT.");

    test_interrupt_handling();

    return 0;
}