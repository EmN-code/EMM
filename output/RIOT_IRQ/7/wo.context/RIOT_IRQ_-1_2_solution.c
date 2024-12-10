#include <stdio.h>
#include <irq_arch.h>  // Include irq header for interrupt handling

volatile unsigned irq_state;

// Function to enable interrupts
unsigned irq_enable(void) {
    unsigned state;
    __asm__ __volatile__("rsil %0, 0" : "=a"(state)); // Disable interrupts
    return state;
}

// Function to restore interrupts
void irq_restore(unsigned state) {
    __asm__ __volatile__("wsr %0, ps; rsync" :: "a"(state)); // Restore previous interrupt state
}

int main(void) {
    // Store initial irq state
    printf("Disabling interrupts...\n");
    irq_state = irq_enable();

    // Critical section starts
    printf("Performing critical operations...\n");
    // Critical section ends

    // Restoring irq state
    printf("Restoring interrupts...\n");
    irq_restore(irq_state);

    printf("Interrupts have been restored.");

    return 0;
}