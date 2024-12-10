
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "irq.h"
#include "thread.h"
#include "event/thread.h"

// Define a simple atomic variable type
typedef volatile int atomic_int_t;

// Function to disable interrupts and return the previous state
unsigned disable_irq(void) {
    return irq_disable();
}

// Function to restore interrupts to a previous state
void restore_irq(unsigned state) {
    irq_restore(state);
}

// Function to simulate an interrupt handler
void interrupt_handler(void) {
    printf("Interrupt occurred!\n");
}

int main(void) {
    // Initialize an atomic variable
    atomic_int_t atomic_var = 0;

    // Disable interrupts and store the previous state
    unsigned irq_state = disable_irq();

    // Simulate some critical section code
    atomic_var = 1;

    // Restore interrupts to the previous state
    restore_irq(irq_state);

    // Simulate an interrupt occurring
    interrupt_handler();

    // Check the atomic variable to verify the critical section was executed
    if (atomic_var == 1) {
        printf("Critical section executed successfully!\n");
    } else {
        printf("Critical section failed!\n");
    }

    return 0;
}
