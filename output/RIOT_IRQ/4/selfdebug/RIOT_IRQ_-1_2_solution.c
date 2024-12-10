#include <stdio.h>  // Include necessary header for printf
#include <irq_arch.h>  // Include the irq header specific to the architecture
#include <stdlib.h>  // Include standard library for other utility functions

volatile atomic_int interrupt_flag = 0;

void irq_handler(void) {
    // Dummy interrupt handler implementation
    printf("Interrupt handled\n");
    atomic_store(&interrupt_flag, 1);
}

int main(void) {
    printf("Program started\n");

    // Disable interrupts and store previous state
    unsigned state = irq_disable();
    
    // Critical section where interrupts are disabled
    printf("Interrupts disabled\n");
    if (atomic_load(&interrupt_flag)) {
        printf("Interrupt occurred while disabled\n");
        atomic_store(&interrupt_flag, 0);  // Reset flag
    }

    // Restore interrupts to previous state
    irq_restore(state);
    printf("Interrupts restored\n");

    return 0;
}