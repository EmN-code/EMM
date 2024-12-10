#include "irq_arch.h"
#include <stdio.h>
#include <stdbool.h>

/* Global atomic variable to track interrupt status */
volatile bool interrupts_enabled = true;

/* Function to simulate the critical section */
void critical_section(void) {
    printf("Entering critical section. Disabling interrupts.\n");
    unsigned state = irq_disable();    // Disable interrupts
    interrupts_enabled = false;
    
    // Perform critical tasks
    printf("Critical section performed with interrupts disabled.\n");

    printf("Re-enabling interrupts.\n");
    irq_restore(state);                // Restore interrupts
    interrupts_enabled = true;
}

int main(void) {
    printf("RIOT ESP32 Interrupt Handling Example\n");

    // Check initial interrupt state
    if (interrupts_enabled) {
        printf("Interrupts are initially enabled.\n");
    } else {
        printf("Interrupts are not enabled as expected.\n");
    }

    // Invoke critical section to test irq functions
    critical_section();

    // Verify final interrupt state
    if (interrupts_enabled) {
        printf("Interrupts were successfully restored.\n");
    } else {
        printf("Interrupts are not restored as expected.\n");
    }

    return 0;
}