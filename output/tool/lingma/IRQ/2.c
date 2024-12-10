#include "xtimer.h"
#include "irq.h"
#include "atomic.h"

// Define an atomic variable to track the interrupt status
atomic_t interrupt_triggered = ATOMIC_INIT(0);

// Define an interrupt handler function
void interrupt_handler(void *arg) {
    (void)arg; // Unused argument
    atomic_set(&interrupt_triggered, 1); // Set the atomic variable to indicate the interrupt was triggered
}

// Main function
int main(void) {
    // Initialize the system
    puts("Starting interrupt handling example");

    // Disable interrupts
    irq_disable();

    // Simulate some critical section where interrupts should not occur
    xtimer_usleep(100000); // Sleep for 100 milliseconds

    // Enable interrupts
    irq_enable();

    // Trigger an interrupt (simulated here)
    interrupt_handler(NULL);

    // Check if the interrupt was triggered
    if (atomic_read(&interrupt_triggered) == 1) {
        puts("Interrupt was triggered");
    } else {
        puts("Interrupt was not triggered");
    }

    // Reset the atomic variable
    atomic_set(&interrupt_triggered, 0);

    // Disable interrupts again
    irq_disable();

    // Simulate another critical section
    xtimer_usleep(100000); // Sleep for 100 milliseconds

    // Enable interrupts
    irq_enable();

    // Trigger the interrupt again
    interrupt_handler(NULL);

    // Check if the interrupt was triggered again
    if (atomic_read(&interrupt_triggered) == 1) {
        puts("Interrupt was triggered again");
    } else {
        puts("Interrupt was not triggered again");
    }

    // Infinite loop to keep the program running
    while (1) {
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    return 0;
}