
#include <stdio.h>
#include "irq.h"
#include "thread.h"

// Define a simple atomic variable type
typedef volatile int atomic_int_t;

// Function prototypes
void irq_handler(void);

// Global atomic variable
atomic_int_t atomic_var = 0;

int main(void)
{
    // Disable interrupts initially
    unsigned state = irq_disable();

    // Store a value in the atomic variable
    atomic_var = 1;

    // Restore interrupts to the previous state
    irq_restore(state);

    // Enable interrupts
    state = irq_enable();

    // Simulate an interrupt by calling the handler directly
    irq_handler();

    // Disable interrupts again
    state = irq_disable();

    // Check the value of the atomic variable
    if (atomic_var == 2) {
        printf("Interrupt handling and atomic operations worked correctly.\n");
    } else {
        printf("Interrupt handling or atomic operations failed.\n");
    }

    // Restore interrupts to the previous state
    irq_restore(state);

    return 0;
}

// Simple interrupt handler
void irq_handler(void)
{
    // Disable interrupts to ensure atomicity
    unsigned state = irq_disable();

    // Increment the atomic variable
    atomic_var++;

    // Restore interrupts to the previous state
    irq_restore(state);
}
