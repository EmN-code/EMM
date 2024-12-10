
#include <stdio.h>
#include <stdbool.h>
#include "irq.h"
#include "thread.h"

// Atomic variable to verify interrupt handling
volatile bool interrupt_handled = false;

// Function to handle the interrupt
void irq_handler(void)
{
    // Set the atomic variable to true when interrupt is handled
    interrupt_handled = true;
}

int main(void)
{
    // Disable interrupts initially
    unsigned state = irq_disable();

    // Register the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler);

    // Enable the interrupt
    irq_enable(IRQ_NUM);

    // Simulate an interrupt by triggering it manually
    irq_trigger(IRQ_NUM);

    // Restore the interrupt state
    irq_restore(state);

    // Check if the interrupt was handled
    if (interrupt_handled) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
