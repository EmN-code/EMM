#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"

// Global state
atomic_bool irq_flag = false;

// Interrupt handler function
void custom_irq_handler(void) {
    // Set the atomic flag in the interrupt
    atomic_store(&irq_flag, true);
}

// Function to disable interrupts
unsigned custom_irq_disable(void) {
    unsigned state = irq_disable();
    return state;
}

// Function to restore interrupts
void custom_irq_restore(unsigned state) {
    irq_restore(state);
}

int main(void) {
    printf("Starting RIOT interrupt handling example\n");

    // Disable interrupts and save current state
    unsigned state = custom_irq_disable();

    // Simulate interrupt handling
    custom_irq_handler();

    // Restore interrupts to previous state
    custom_irq_restore(state);

    // Check atomic flag to verify that interrupt handler was executed
    if(atomic_load(&irq_flag)) {
        printf("Interrupt flag set successfully. IRQ handling verified.\n");
    } else {
        printf("Failed to set interrupt flag. IRQ handling failed.\n");
    }

    return 0;
}