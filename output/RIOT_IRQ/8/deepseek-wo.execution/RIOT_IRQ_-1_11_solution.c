
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <irq.h>

volatile bool interrupt_occurred = false;

void interrupt_handler(void) {
    interrupt_occurred = true;
}

int main(void) {
    unsigned state;

    // Disable interrupts
    state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(interrupt_handler);

    // Simulate an interrupt occurring
    interrupt_occurred = false;

    // Restore interrupts
    irq_restore(state);

    // Wait for the interrupt to be handled
    while (!interrupt_occurred) {
        // Busy wait
    }

    printf("Interrupt handled successfully!\n");

    return 0;
}
