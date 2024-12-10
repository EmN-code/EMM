#include <xtimer.h>
#include <stdio.h>
#include "irq_arch.h"

static volatile unsigned irq_disabled_status = 0;

void irq_handler_example(void) {
    unsigned irq_status;
    
    // Disable interrupts and save the status
    irq_status = irq_disable();
    printf("Interrupts disabled!\n");
    
    // Simulate some work in the critical section
    for (volatile int i = 0; i < 1000000; i++);
    
    // Increment the atomic variable
    irq_disabled_status++;
    
    // Restore interrupts
    irq_restore(irq_status);
    printf("Interrupts restored!\n");
}

int main(void) {
    printf("Starting interrupt handler example...\n");
    
    // Example of manually invoking the interrupt handler
    irq_handler_example();

    // Output the result of the interrupt status
    printf("IRQ Disabled Status: %u\n", irq_disabled_status);

    return 0;
}