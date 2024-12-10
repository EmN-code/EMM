#include "xtimer.h"
#include "irq.h"
#include "atomic.h"
#include "stdio.h"

// Atomic variable to track interrupt status
atomic_t interrupt_flag = ATOMIC_INIT(0);

// Function to handle the interrupt
void interrupt_handler(void *arg) {
    (void)arg; // Unused argument

    // Atomically set the interrupt flag
    atomic_set(&interrupt_flag, 1);

    // Print a message indicating the interrupt occurred
    printf("Interrupt occurred!\n");
}

// Function to simulate an interrupt source
void simulate_interrupt(void) {
    // Simulate an interrupt by calling the handler
    interrupt_handler(NULL);
}

int main(void) {
    // Initialize the system
    printf("Starting interrupt handling example...\n");

    // Disable global interrupts
    irq_disable();

    // Simulate an interrupt
    simulate_interrupt();

    // Check the interrupt flag atomically
    if (atomic_read(&interrupt_flag) == 1) {
        printf("Interrupt flag is set.\n");
    } else {
        printf("Interrupt flag is not set.\n");
    }

    // Restore global interrupts
    irq_restore();

    // Main loop
    while (1) {
        // Perform some operations
        xtimer_sleep(1); // Sleep for 1 second
    }

    return 0;
}