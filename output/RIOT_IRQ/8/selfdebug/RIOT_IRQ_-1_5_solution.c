#include <stdio.h>
#include <stdlib.h>
#include "irq.h"
#include "kernel_types.h"

// Function to simulate an interrupt handler that increments an atomic variable
void some_handler(void) {
    static _Atomic int count = 0; // Atomic variable to store interrupt count
    count++;
    printf("Interrupt count: %d\n", count);  // Print the number of interrupts handled
}

int main(void) {
    // Initially disable interrupts
    unsigned state = irq_disable();

    // Simulate triggering the interrupt handler
    for (int i = 0; i < 5; i++) {
        some_handler();
    }

    // Restore the state of interrupts
    irq_restore(state);

    // Enable interrupts
    irq_enable();

    // Simulate triggering the interrupt handler again after interrupts enabled
    for (int i = 0; i < 5; i++) {
        some_handler();
    }

    return 0;
}