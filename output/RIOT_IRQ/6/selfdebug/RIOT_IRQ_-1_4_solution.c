#include <assert.h> // Including in descending order by first letter
#include <irq_arch.h> // Required for IRQ handling functions
#include <stdio.h>

// Initialize an atomic variable
volatile int irq_state;
volatile int critical_section = 0;

void critical_task(void) {
    // Simulating a task that needs interrupt protection
    for (int i = 0; i < 5; i++) {
        printf("Executing critical task iteration %d\n", i);
    }
}

int main(void) {
    // Enable interrupts and save previous state
    irq_state = irq_enable();
    
    // Some code that runs with interrupts enabled
    printf("Running with interrupts enabled.\n");

    // Disable interrupts
    irq_disable();
    
    // Simulate critical section
    if (critical_section == 0) {
        critical_section = 1;
        critical_task();
        critical_section = 0;
    }

    // Restore interrupts to previous state
    irq_restore(irq_state);

    // Confirm rest of the program runs
    printf("Interrupts restored, exiting program.\n");

    return 0;
}