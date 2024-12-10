#include "irq_arch.h"

// Initialize an atomic variable
static _Atomic bool irq_state;

// A placeholder function to simulate some work
void perform_work(void) {
    // Simulate workload
    for (volatile int i = 0; i < 10000; ++i) {
        // Busy-wait to simulate load
    }
}

int main(void) {
    unsigned state_before, state_after;

    // Disable IRQ and store the state
    state_before = irq_disable();
    irq_state = state_before;
    
    // Perform some work with interrupts disabled
    perform_work();

    // Restore the IRQ state
    irq_restore(irq_state);
    state_after = irq_is_enabled() ? 1 : 0;

    // Output the state of the interrupt handling
    if (state_before != state_after) {
        printf("Interrupt state mismatch: was %u, now %u\n", state_before, state_after);
    } else {
        printf("Interrupt state consistent: %u\n", state_before);
    }

    return 0;
}