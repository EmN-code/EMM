#include "irq.h"
#include <stdio.h>
#include <stdatomic.h>

volatile atomic_uint global_counter = 0;

void interrupt_handler(void) {
    // Simulate some work done in the interrupt
    atomic_fetch_add(&global_counter, 1);
}

void main(void) {
    unsigned int irq_state;

    // Disable interrupts
    irq_state = irq_disable();

    // Simulate critical section
    printf("Interrupts disabled, critical section start\n");

    // Register the interrupt handler
    // Note: This is a placeholder, actual registration depends on your setup
    // register_interrupt_handler(interrupt_handler);

    // Simulate some work
    for (int i = 0; i < 1000; i++) {
        atomic_fetch_add(&global_counter, 1);
    }

    // Restore interrupts
    irq_restore(irq_state);

    printf("Interrupts restored, critical section end\n");
    printf("Global counter: %u\n", atomic_load(&global_counter));
}