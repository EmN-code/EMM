
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "irq.h"

volatile uint32_t atomic_var = 0;

void interrupt_handler(void) {
    unsigned state = irq_disable();
    atomic_var++;
    irq_restore(state);
}

int main(void) {
    printf("Starting interrupt handling test...\n");

    // Disable interrupts initially
    unsigned state = irq_disable();

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Busy loop
    }

    // Enable interrupts and trigger the handler
    irq_restore(state);
    interrupt_handler();

    // Check the atomic variable
    if (atomic_var == 1) {
        printf("Interrupt handling test passed!\n");
    } else {
        printf("Interrupt handling test failed!\n");
    }

    return 0;
}
