#include <stdio.h>
#include <stdatomic.h>  // Correct header for atomic_bool
#include <irq.h>

volatile atomic_bool irq_disabled;  // Declare an atomic_bool variable

void check_interrupt_handling(void) {
    // Disable interrupts
    irq_disabled = 1;
    irq_disable();

    // Simulate critical section
    printf("Interrupts disabled\n");

    // Restore interrupts
    irq_disabled = 0;
    irq_restore();
    printf("Interrupts restored\n");
}

int main(void) {
    // Initial state
    printf("Starting interrupt handling check\n");
 
    // Run the check
    check_interrupt_handling();

    // Check atomic variable state
    if (irq_disabled) {
        printf("Error: interrupts are still disabled!\n");
    } else {
        printf("Interrupt handling works correctly.\n");
    }

    return 0;
}