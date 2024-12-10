#include <irq_arch.h>
#include <assert.h>
#include <stdio.h>

volatile unsigned irq_status;

void test_irq_handling(void) {
    // Disable interrupts and store the current status
    irq_status = irq_disable();
    printf("Interrupts disabled. Status stored: %u\n", irq_status);
    
    // Simulate some critical operation here
    // ...

    // Restore interrupts to previous status
    irq_restore(irq_status);
    printf("Interrupts restored.\n");
}

int main(void) {
    printf("Starting IRQ handling test program\n");

    // Test IRQ handling
    test_irq_handling();

    printf("IRQ handling test program completed\n");
    return 0;
}