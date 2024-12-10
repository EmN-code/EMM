
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <event/thread.h>

// Define a simple atomic variable type
typedef volatile int atomic_t;

// Define a constant for atomic initialization
#define ATOMIC_INIT(val) (val)

// Function to set the value of an atomic variable
void atomic_set(atomic_t *var, int val) {
    *var = val;
}

// Function to get the value of an atomic variable
int atomic_get(atomic_t *var) {
    return *var;
}

// Define a simple interrupt handler
void my_interrupt_handler(void) {
    printf("Interrupt handled!\n");
}

int main(void) {
    // Initialize an atomic variable
    atomic_t my_atomic = ATOMIC_INIT(0);

    // Set the atomic variable to 1
    atomic_set(&my_atomic, 1);

    // Disable interrupts
    unsigned int state = irq_disable();

    // Check if interrupts are disabled
    if (atomic_get(&my_atomic) == 1) {
        printf("Interrupts are disabled.\n");
    }

    // Restore interrupts to their previous state
    irq_restore(state);

    // Set the interrupt handler
    irq_set_handler(0, my_interrupt_handler);

    // Enable the interrupt
    irq_enable(0);

    // Check if interrupts are enabled
    if (atomic_get(&my_atomic) == 1) {
        printf("Interrupts are enabled.\n");
    }

    // Wait for the interrupt to be handled
    while (1) {
        // Busy-wait loop
    }

    return 0;
}
