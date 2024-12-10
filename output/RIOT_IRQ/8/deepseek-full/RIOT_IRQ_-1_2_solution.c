
#include <stdio.h>
#include <stdbool.h>
#include "irq.h"
#include "thread.h"
#include "event/thread.h"

// Define an atomic variable to track interrupt handling
volatile bool interrupt_handled = false;

// Function to simulate an interrupt
void simulate_interrupt(void) {
    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate some work being done
    printf("Interrupt simulated, performing work...\n");

    // Mark the interrupt as handled
    interrupt_handled = true;

    // Restore interrupts to their previous state
    irq_restore(state);
}

// Main function
int main(void) {
    printf("Starting interrupt handling test...\n");

    // Simulate an interrupt
    simulate_interrupt();

    // Verify if the interrupt was handled correctly
    if (interrupt_handled) {
        printf("Interrupt handling verified successfully.\n");
    } else {
        printf("Interrupt handling verification failed.\n");
    }

    return 0;
}
