
#include <stdio.h>
#include <stdbool.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

// Atomic variable to verify interrupt handling
volatile bool interrupt_handled = false;

// Function to handle the interrupt
void interrupt_handler(void)
{
    // Set the atomic variable to true
    interrupt_handled = true;
}

// Function to simulate an interrupt
void simulate_interrupt(void)
{
    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate an interrupt by calling the handler
    interrupt_handler();

    // Restore interrupts
    irq_restore(state);
}

int main(void)
{
    // Initialize the atomic variable
    interrupt_handled = false;

    // Print initial state
    printf("Initial state: interrupt_handled = %d\n", interrupt_handled);

    // Simulate an interrupt
    simulate_interrupt();

    // Print the state after the interrupt
    printf("State after interrupt: interrupt_handled = %d\n", interrupt_handled);

    // Verify if the interrupt was handled correctly
    if (interrupt_handled) {
        printf("Interrupt handling verified successfully.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    return 0;
}
