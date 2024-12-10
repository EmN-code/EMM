
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "irq.h"

volatile bool interrupt_occurred = false;

void irq_handler(void) {
    interrupt_occurred = true;
}

int main(void) {
    unsigned state;

    // Disable interrupts
    state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(irq_handler);

    // Restore interrupts to the previous state
    irq_restore(state);

    // Enable interrupts
    irq_enable();

    // Main loop to check if the interrupt occurred
    while (1) {
        if (interrupt_occurred) {
            printf("Interrupt occurred!\n");
            interrupt_occurred = false;
        }
    }

    return 0;
}
