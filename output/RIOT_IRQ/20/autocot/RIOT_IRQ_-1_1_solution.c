#include "irq_arch.h"
#include <stdint.h>
#include <stdio.h>

// Atomic variable to indicate interrupt state
volatile uint32_t irq_status = 0;

// Function to simulate work while interrupts are enabled
void do_work(void) {
    printf("Doing work with interrupts enabled.\n");
}

// Function to handle interrupts by saving and restoring irq state
void handle_interrupts(void) {
    // Disable interrupts and save the status
    uint32_t status = irq_disable();
    printf("Interrupts disabled. Status saved: %u\n", status);
    
    // Simulate critical work that requires interrupts to be disabled
    printf("Handling critical work with interrupts disabled.\n");

    // Restore interrupts
    irq_restore(status);
    printf("Interrupts restored to state: %u\n", status);
    
    // Store status in atomic variable
    irq_status = status;
}

// Main function
int main(void) {
    // Make sure interrupts are enabled initially
    irq_status = irq_is_enabled();
    printf("Initial interrupt status: %u\n", irq_status);
    
    // Perform some work with interrupts enabled
    do_work();

    // Handle interrupts (disable, perform critical tasks, restore)
    handle_interrupts();

    return 0;
}