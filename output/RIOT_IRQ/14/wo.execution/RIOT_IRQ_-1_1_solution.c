#include <irq_arch.h>

#include <stdio.h>
#include <stdlib.h>

// Atomic variable for interrupt testing
volatile unsigned irq_state;

// Function to enable an interrupt, using pseudo implementation 
static inline void enable_interrupt(void) {
    // Code to enable interrupt
    printf("Interrupt enabled.\n");
}

// Function to disable an interrupt and return its previous state, using pseudo implementation
static inline unsigned disable_interrupt(void) {
    // Pseudo code to disable the interrupt and set irq_state
    unsigned state = irq_state;
    irq_state = 0;
    printf("Interrupt disabled.\n");
    return state;
}

// Function to restore the interrupt to its previous state, using pseudo implementation
static inline void restore_interrupt(unsigned state) {
    irq_state = state;
    printf("Interrupt restored.\n");
}

int main(void) {
    printf("Starting interrupt handling test on ESP32.\n");

    // Disabling interrupts
    unsigned previous_state = disable_interrupt();

    // Hypothetical test to illustrate the change in state
    printf("Performing some operations while interrupt is disabled.\n");

    // Restore the interrupt to its original state
    restore_interrupt(previous_state);

    // Final test to check the state
    if (irq_state != 0) {
        printf("Interrupt handling works correctly.\n");
    } else {
        printf("There was a problem restoring interrupt state.\n");
    }

    return EXIT_SUCCESS;
}