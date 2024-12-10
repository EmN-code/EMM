#include "ztimer.h"
#include "irq_arch.h"
#include <stdio.h>
#include <stdbool.h>

static volatile bool interrupt_occurred = false;

void interrupt_handler(void) {
    interrupt_occurred = true;
}

int main(void) {
    printf("Interrupt Handling Example with Atomic Variables\n");
    
    unsigned state;
    
    // Disable interrupts and save the current state
    state = irq_disable();

    // Simulate interrupt handling
    interrupt_handler();

    // Restore interrupts to previous state
    irq_restore(state);

    // Check if interrupt was handled
    if (interrupt_occurred) {
        printf("Interrupt was successfully handled and restored.\n");
    }
    else {
        printf("Interrupt was not handled correctly.\n");
    }

    return 0;
}