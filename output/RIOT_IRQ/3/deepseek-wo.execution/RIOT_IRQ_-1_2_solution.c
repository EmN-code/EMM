
#include "irq.h"
#include "mma8x5x_regs.h"

volatile unsigned int atomic_var = 0;

void interrupt_handler(void) {
    atomic_var = 1; // Set atomic variable to indicate interrupt occurred
}

int main(void) {
    unsigned state;

    // Disable interrupts
    state = irq_disable();

    // Perform some critical section code
    // For example, increment a counter
    atomic_var = 0;

    // Restore interrupts to the previous state
    irq_restore(state);

    // Enable interrupts
    state = irq_enable();

    // Simulate an interrupt occurring
    interrupt_handler();

    // Disable interrupts again to check the atomic variable
    state = irq_disable();

    // Check if the interrupt handler set the atomic variable
    if (atomic_var == 1) {
        // Interrupts were correctly restored and the handler was called
        // Perform some action to indicate success
    } else {
        // Interrupts were not correctly restored or the handler was not called
        // Perform some action to indicate failure
    }

    // Restore interrupts to the previous state
    irq_restore(state);

    return 0;
}
