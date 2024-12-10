#include "irq_arch.h"
#include <stdio.h>
#include <stdatomic.h>
#include <stdint.h>

void handle_interrupt(void);

int main(void) {
    // Initialize atomic variable
    atomic_int interrupt_handling_status = ATOMIC_VAR_INIT(0);
    printf("Interrupt Handler Demonstration\n");
    // Disable interrupts and update status atomically
    irq_disable();
    atomic_store(&interrupt_handling_status, 1);
    printf("Interrupts are disabled.\n");

    // Critical section of the code where interrupts should not interfere
    handle_interrupt();

    // Restore interrupts and update status atomically
    irq_restore(irq_is_enabled());
    atomic_store(&interrupt_handling_status, 0);
    printf("Interrupts are restored.\n");

    // Check if interrupts are enabled
    if (irq_is_enabled()) {
        printf("Interrupts are enabled correctly.\n");
    }
    return 0;
}

void handle_interrupt(void) {
    // Simulate doing some critical work
    printf("Handling critical section with interrupts disabled.\n");
}