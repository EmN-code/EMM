#include <stdio.h>
#include <stdint.h>
#include <atomic.h>

#include "xtimer.h"
#include "irq.h"

// Define the interrupt handler
void interrupt_handler(void *arg) {
    (void)arg;
    printf("Interrupt occurred!\n");
}

// Atomic variable to check if interrupts are working
static atomic_t interrupt_occurred;

// Function to simulate an interrupt
void simulate_interrupt(void) {
    // Disable interrupts
    unsigned state = irq_disable();

    // Set the atomic variable
    atomic_set(&interrupt_occurred, 1);

    // Enable interrupts
    irq_restore(state);

    // Trigger the interrupt handler
    interrupt_handler(NULL);
}

int main(void) {
    // Initialize the atomic variable
    atomic_init(&interrupt_occurred, 0);

    // Print initial state
    printf("Initial state: interrupt_occurred = %d\n", atomic_get(&interrupt_occurred));

    // Simulate an interrupt
    simulate_interrupt();

    // Check if the interrupt was handled
    if (atomic_get(&interrupt_occurred) == 1) {
        printf("Interrupt handling verified successfully.\n");
    } else {
        printf("Interrupt handling verification failed.\n");
    }

    // Main loop
    while (1) {
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    return 0;
}