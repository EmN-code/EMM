#include <stdbool.h>  // Ensure to include the proper header for bool
#include <stdio.h>    // Ensure to include this for printf
#include <irq_arch.h> // Include the relevant RIOT headers for IRQ

// Function to handle enabling and disabling interrupts
unsigned irq_handler(void) {
    // Disable interrupts and capture the state
    unsigned state = irq_disable();
    printf("Interrupts disabled.\n");

    // Some operations that require interrupts to be disabled

    // Restore previous state
    irq_restore(state);
    printf("Interrupts restored.\n");

    return state;  // Return the state to maybe reuse this information
}

int main(void) {
    // Call the irq handler and verify functionality
    unsigned state = irq_handler();
    printf("Main function executed with IRQ state: %u\n", state);

    return 0;
}