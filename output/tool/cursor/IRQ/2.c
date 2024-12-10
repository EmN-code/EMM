#include "irq.h"
#include "atomic_utils.h"
#include <stdio.h>

volatile uint32_t global_counter = 0;

void interrupt_handler(void) {
    // This function will be called when an interrupt occurs
    atomic_add_u32(&global_counter, 1);
}

void setup_interrupts(void) {
    // Disable interrupts
    unsigned int state = irq_disable();

    // Set up your interrupt handler here
    // For example, you might configure a timer or another peripheral to generate interrupts
    // and associate `interrupt_handler` with that interrupt source.

    // Restore interrupts
    irq_restore(state);
}

void check_interrupts(void) {
    // This function checks if the interrupt handling is working correctly
    if (atomic_load_u32(&global_counter) > 0) {
        printf("Interrupts are working correctly. Counter: %u\n", global_counter);
        atomic_store_u32(&global_counter, 0);
    } else {
        printf("No interrupts detected.\n");
    }
}

int main(void) {
    setup_interrupts();

    while (1) {
        check_interrupts();
        // Add a delay or other logic here as needed
    }

    return 0;
}