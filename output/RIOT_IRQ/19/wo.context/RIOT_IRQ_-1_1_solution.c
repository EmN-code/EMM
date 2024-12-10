#include "lwip/pbuf.h"
#include "irq_arch.h"

#include <stdio.h>
#include <stdbool.h>

#define CRITICAL_VAR_INIT_VALUE 0

volatile int critical_var = CRITICAL_VAR_INIT_VALUE;

static inline void irq_test_function(void) {
    unsigned state;
    bool irq_enabled_before, irq_enabled_after;

    // Check if interrupts are enabled before
    irq_enabled_before = irq_is_enabled();

    // Disable interrupts
    state = irq_disable();

    // Critical section
    critical_var++;

    // Restore interrupts
    irq_restore(state);

    // Check if interrupts are enabled after
    irq_enabled_after = irq_is_enabled();

    // Print the results
    printf("Interrupts enabled before: %d\n", irq_enabled_before);
    printf("Interrupts enabled after: %d\n", irq_enabled_after);
    printf("Critical variable value: %d\n", critical_var);
}

int main(void) {
    printf("Testing IRQ handling in RIOT on ESP32\n");
    irq_test_function();
    return 0;
}