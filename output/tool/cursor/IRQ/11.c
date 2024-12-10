#include "irq.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// Atomic variable to track interrupt state
volatile uint32_t interrupt_state = 0;

// Function to disable interrupts and store the state
void disable_interrupts(void) {
    interrupt_state = irq_disable();
    printf("Interrupts disabled, state saved: %u\n", interrupt_state);
}

// Function to restore interrupts to the previous state
void restore_interrupts(void) {
    irq_restore(interrupt_state);
    printf("Interrupts restored to state: %u\n", interrupt_state);
}

// Main function
int main(void) {
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    disable_interrupts();

    // Simulate some critical section code
    printf("Critical section: interrupts are disabled.\n");

    // Restore interrupts
    restore_interrupts();

    printf("Interrupt handling test completed.\n");

    return 0;
}