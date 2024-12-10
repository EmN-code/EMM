#include "xtimer.h"
#include "irq.h"
#include "thread.h"

// Atomic variable to check interrupt handling
volatile uint32_t interrupt_flag = 0;

// Function to simulate an interrupt handler
void simulated_interrupt_handler(void) {
    // Disable interrupts
    unsigned state = irq_disable();

    // Set the atomic variable
    interrupt_flag = 1;

    // Restore interrupts
    irq_restore(state);

    // Print a message to indicate the interrupt was handled
    printf("Interrupt handled\n");
}

// Main function
int main(void) {
    // Initialize the system
    puts("Starting interrupt handling example");

    // Simulate an interrupt
    simulated_interrupt_handler();

    // Check if the interrupt was handled correctly
    if (interrupt_flag == 1) {
        printf("Interrupt handling verified successfully\n");
    } else {
        printf("Interrupt handling verification failed\n");
    }

    // Main loop
    while (1) {
        // Sleep for a while
        xtimer_sleep(1);
    }

    return 0;
}