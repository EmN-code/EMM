#include "irq_arch.h"

volatile bool interrupt_flag = false;

void interrupt_handler(void) {
    // Simple interrupt handler which toggles the interrupt flag
    interrupt_flag = !interrupt_flag;
}

int main(void) {
    // First, let's disable interrupts
    unsigned state = irq_disable();

    // Simulate an interrupt happening
    interrupt_handler();

    // Check if the interrupt flag wasn't set because interrupts were disabled
    if (interrupt_flag) {
        return 1; // There is an error if interrupt flag is set
    }

    // Now, re-enable interrupts
    irq_restore(state);

    // Simulate the interrupt again
    interrupt_handler();

    // Check that the interrupt handler was successfully called
    if (!interrupt_flag) {
        return 1; // There is an error if interrupt flag is not set
    }

    return 0;
}