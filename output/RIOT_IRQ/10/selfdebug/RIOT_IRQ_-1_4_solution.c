#include <stdio.h>
#include <irq_arch.h>
#include <stdbool.h>

void irq_example(void) {
    // Simulate atomic variable
    volatile bool irq_disabled = false;

    // Save current interrupt state and disable interrupts
    unsigned state = irq_disable();
    irq_disabled = true;

    // Simulated critical section where interrupts are disabled
    if (irq_disabled) {
        printf("Interrupts are successfully disabled. Entering critical section.\n");
    }
    
    // Restore interrupts
    irq_restore(state);
    irq_disabled = false;
    
    if (!irq_disabled) {
        printf("Interrupts are restored successfully. Exiting critical section.\n");
    }
}

int main(void) {
    printf("Testing interrupt handling on ESP32 with RIOT.\n");
    irq_example();
    return 0;
}