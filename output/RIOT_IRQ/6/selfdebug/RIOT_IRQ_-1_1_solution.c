#include <stdbool.h>
#include <irq_arch.h>

static volatile unsigned irq_enabled_state;

// Function to disable interrupts and store the current state
void disable_interrupts(void) {
    irq_enabled_state = irq_disable();
}

// Function to restore interrupts to the previously saved state
void restore_interrupts(void) {
    if (irq_enabled_state) {
        __enable_irq();
    }
}

int main(void) {
    // Atomic variable to check the state of interrupts
    volatile bool is_irq_enabled;

    // Initially checking if interrupts are enabled
    is_irq_enabled = irq_is_enabled();

    if (is_irq_enabled) {
        printf("Interrupts are currently enabled.\n");
    } else {
        printf("Interrupts are currently disabled.\n");
    }

    // Disable interrupts
    disable_interrupts();
    is_irq_enabled = irq_is_enabled();

    if (!is_irq_enabled) {
        printf("Successfully disabled interrupts.\n");
    } else {
        printf("Failed to disable interrupts.\n");
    }

    // Restore interrupts
    restore_interrupts();
    is_irq_enabled = irq_is_enabled();

    if (is_irq_enabled) {
        printf("Successfully restored interrupts.\n");
    } else {
        printf("Failed to restore interrupts.\n");
    }

    return 0;
}