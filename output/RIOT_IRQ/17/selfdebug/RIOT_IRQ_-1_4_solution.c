#include "irq_arch.h"
#include "xtimer.h"
#include <stdio.h>

volatile int shared_var = 0;

void isr_handler(void) {
    // This function will act as an interrupt service routine (ISR)
    shared_var++;  // Increment the shared variable in an atomic way
}

int main(void) {
    unsigned state;

    // Disable interrupts and save the state
    state = irq_disable();

    // Simulate a critical section where interrupts are disabled
    printf("Interrupts disabled. Current shared_var: %d\n", shared_var);
    
    // Enable interrupts and restore to the previous state
    irq_restore(state);
    
    printf("Interrupts restored. Current shared_var: %d\n", shared_var);
    
    // Set up a timer to trigger the interrupt handler
    xtimer_usleep(1000000); // Sleep to simulate waiting for the interrupt
    isr_handler(); // Direct call to simulate interrupt handler action

    printf("After ISR. Current shared_var: %d\n", shared_var);

    return 0;
}