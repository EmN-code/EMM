#include "irq_arch.h"
#include <stdio.h>

volatile unsigned irq_status;
volatile int interrupt_counter = 0;

void irq_handler(void) {
    interrupt_counter++;
    printf("Interrupt occurred! Counter: %d\n", interrupt_counter);
}

int main(void) {
    irq_status = irq_disable();

    printf("Interrupts disabled, status: %u\n", irq_status);

    // Simulate an interrupt after disabling
    irq_handler();  // This would be ignored in a real situation
    
    irq_restore(irq_status);
    
    printf("Interrupts restored.");
    
    // Simulate an interrupt after restoring
    irq_handler();  // Now it should be handled

    return 0;
}