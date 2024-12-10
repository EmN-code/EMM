#include "xtimer.h"
#include "irq.h"
#include "thread.h"

// Atomic variable to check if the interrupt handler has been called
volatile bool interrupt_triggered = false;

// Interrupt handler function
void interrupt_handler(void *arg) {
    (void)arg; // Unused parameter
    xtimer_now();
    interrupt_triggered = true;
}

// Main function
int main(void) {
    // Initialize the system
    puts("Starting interrupt handling example");

    // Disable interrupts
    irq_disable();

    // Simulate some critical section where interrupts are disabled
    xtimer_usleep(1000); // Sleep for 1 ms

    // Enable interrupts
    irq_enable();

    // Set up the interrupt
    // Note: In a real scenario, you would set up the specific interrupt source here
    // For demonstration, we will simulate an interrupt trigger
    puts("Enabling interrupt handler");
    xtimer_call(interrupt_handler, NULL, 1000000); // Call the interrupt handler after 1 second

    // Wait for the interrupt to be triggered
    while (!interrupt_triggered) {
        xtimer_usleep(1000); // Sleep for 1 ms
    }

    // Check if the interrupt was handled
    if (interrupt_triggered) {
        puts("Interrupt handler was triggered successfully");
    } else {
        puts("Interrupt handler was not triggered");
    }

    // Clean up and exit
    return 0;
}