#include "xtimer.h"
#include "irq.h"
#include "atomic.h"
#include "stdio.h"

// Atomic variable to check interrupt status
atomic_t interrupt_flag = ATOMIC_INIT(0);

// Function to handle the interrupt
void interrupt_handler(void) {
    // Set the atomic flag to indicate the interrupt occurred
    atomic_set(&interrupt_flag, 1);
    printf("Interrupt occurred!\n");
}

// Function to simulate an interrupt
void simulate_interrupt(void) {
    // Simulate an interrupt by calling the handler
    interrupt_handler();
}

int main(void) {
    // Initialize the system
    printf("Starting interrupt handling example...\n");

    // Disable interrupts
    irq_disable();

    // Check the initial state of the atomic flag
    if (atomic_read(&interrupt_flag) == 0) {
        printf("Interrupt flag is initially 0.\n");
    }

    // Simulate an interrupt
    simulate_interrupt();

    // Check the state of the atomic flag after simulating the interrupt
    if (atomic_read(&interrupt_flag) == 1) {
        printf("Interrupt flag is set to 1 after simulating the interrupt.\n");
    }

    // Restore interrupts
    irq_restore(irq_disable());

    // Check the state of the atomic flag after restoring interrupts
    if (atomic_read(&interrupt_flag) == 1) {
        printf("Interrupt flag remains 1 after restoring interrupts.\n");
    }

    // Main loop
    while (1) {
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    return 0;
}