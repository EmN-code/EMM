
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "irq.h"

// Define a simple atomic type and functions to simulate atomic operations
typedef volatile uint32_t atomic_t;

#define ATOMIC_INIT(val) (val)

static inline void atomic_store(atomic_t *ptr, uint32_t val) {
    *ptr = val;
}

static inline uint32_t atomic_load(atomic_t *ptr) {
    return *ptr;
}

// Global atomic variable to test interrupt handling
atomic_t atomic_var = ATOMIC_INIT(0);

// Interrupt handler function
void my_interrupt_handler(void) {
    atomic_store(&atomic_var, 1); // Set atomic variable to 1 when interrupt occurs
}

int main(void) {
    // Enable interrupts globally
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(my_interrupt_handler);

    // Restore the interrupt state
    irq_restore(state);

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Do some work
    }

    // Check the atomic variable to verify if the interrupt was handled
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupt was successfully handled!\n");
    } else {
        printf("Interrupt was not handled!\n");
    }

    return 0;
}
