
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <event/thread.h>
#include <irq.h>
#include <thread.h>

// Define an atomic variable type
typedef volatile uint32_t atomic_t;

// Define a macro to initialize atomic variables
#define ATOMIC_INIT(val) (val)

// Function to store a value atomically
static inline void atomic_store(atomic_t *var, uint32_t val) {
    *var = val;
}

// Function to load a value atomically
static inline uint32_t atomic_load(atomic_t *var) {
    return *var;
}

// Global atomic variable
atomic_t atomic_var = ATOMIC_INIT(0);

// Interrupt handler function
void irq_handler(void) {
    // Disable interrupts to ensure atomic operation
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_store(&atomic_var, 1);

    // Restore interrupts
    irq_restore(state);
}

int main(void) {
    // Initialize the atomic variable
    atomic_store(&atomic_var, 0);

    // Set the interrupt handler
    irq_set_handler(0, irq_handler);

    // Enable the interrupt
    irq_enable(0);

    // Trigger the interrupt
    irq_trigger(0);

    // Wait for a short period to allow the interrupt to be handled
    for (volatile int i = 0; i < 1000000; i++);

    // Check the value of the atomic variable
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupt handling and atomic operations worked correctly.\n");
    } else {
        printf("Interrupt handling or atomic operations failed.\n");
    }

    return 0;
}
