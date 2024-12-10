#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Define the atomic variable
volatile bool irq_disabled = false;

// Function to simulate atomic disable irq
unsigned irq_disable(void) {
    unsigned state = irq_disabled ? 1 : 0;
    irq_disabled = true;
    return state;
}

// Function to simulate atomic restore irq
void irq_restore(unsigned state) {
    irq_disabled = (state != 0);
}

// Main function
int main() {
    printf("Starting interrupt handling test.\n");

    // Disable interrupts and save the state
    unsigned state = irq_disable();
    printf("Interrupts disabled, state saved: %u\n", state);

    // Verify the interrupts are disabled
    if (irq_disabled) {
        printf("Interrupts are currently disabled.\n");
    } else {
        printf("Interrupts are not disabled as expected.\n");
    }

    // Restore interrupts using saved state
    irq_restore(state);
    printf("Interrupts restored.\n");

    // Verify interrupts are restored
    if (!irq_disabled) {
        printf("Interrupts are currently enabled.\n");
    } else {
        printf("Interrupts are not enabled as expected.\n");
    }

    printf("Interrupt handling test completed.\n");
    return 0;
}