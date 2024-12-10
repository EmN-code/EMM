#include <irq_arch.h>
#include <stdio.h>

volatile bool interrupt_enabled;

// Function to simulate enabling of interrupt
void enable_interrupt(void) {
    printf("Interrupt enabled.\n");
    interrupt_enabled = true;
}

// Function to simulate disabling of interrupt
void disable_interrupt(void) {
    printf("Interrupt disabled.\n");
    interrupt_enabled = false;
}

// Main function
int main(void) {
    printf("Starting interrupt handling test on ESP32.\n");

    // Check initial state of interrupts
    if (irq_is_enabled()) {
        printf("Interrupts initially enabled.\n");
    } else {
        printf("Interrupts initially disabled.\n");
    }

    // Disable interrupts
    unsigned state = irq_disable();
    printf("Interrupt state saved: %u.\n", state);

    // Confirm interrupts are disabled
    if (!irq_is_enabled()) {
        printf("Interrupts are now disabled.\n");
    }

    // Restore previous interrupt state
    irq_restore(state);
    printf("Restored interrupts to previous state.\n");

    // Verify interrupts are enabled again
    if (irq_is_enabled()) {
        printf("Interrupts are enabled again. Test successful.\n");
    } else {
        printf("Failed to restore interrupts.\n");
    }

    return 0;
}