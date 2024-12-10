#include "irq_arch.h"
#include <stdio.h>

atomic_uint interrupt_counter = 0;

// Simulate an interrupt service routine that increments a counter
void interrupt_handler(void) {
    // Atomically increment the interrupt counter
    atomic_fetch_add(&interrupt_counter, 1);
}

int main(void) {
    unsigned state;

    // Simulate disabling interrupts
    state = irq_disable();
    printf("Interrupts disabled, state saved: %u\n", state);

    // Simulate an interrupt occurring by manually invoking the handler
    interrupt_handler();
    printf("Interrupt handler called, counter: %u\n", interrupt_counter);

    // Simulate restoring interrupts
    irq_restore(state);
    printf("Interrupts restored to state: %u\n", state);

    return 0;
}