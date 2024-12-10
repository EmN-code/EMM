#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <irq_arch.h>

volatile bool interrupt_occurred = false;  // Atomic variable to track the interrupt state

void handle_interrupt(void) {
    // Simulate an interrupt occurring
    interrupt_occurred = true;
    printf("Interrupt occurred!\n");
}

int main(void) {
    printf("Starting interrupt handling example.\n");
    
    // Initially, interrupts are enabled
    bool initially_enabled = irq_is_enabled();
    printf("Interrupts initially enabled: %d\n", initially_enabled);
    
    // Disable interrupts and store the state
    unsigned irq_state = irq_disable();
    printf("Interrupts disabled.\n");

    // Trigger a simulated interrupt
    handle_interrupt();

    if (interrupt_occurred) {
        printf("Interrupt handling failed! Interrupts should be disabled.\n");
    } else {
        printf("Interrupt handling successful. No interrupt should have occurred.\n");
    }

    // Restore interrupts
    irq_enable();
    printf("Interrupts restored.\n");

    // Check if the interrupts were restored correctly
    if (irq_is_enabled() != initially_enabled) {
        printf("Interrupts were not restored to their initial state!\n");
    } else {
        printf("Interrupts restored successfully to their initial state.\n");
    }

    return 0;
}