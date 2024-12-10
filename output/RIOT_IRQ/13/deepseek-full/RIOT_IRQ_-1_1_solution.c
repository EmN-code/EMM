
#include <stdio.h>
#include <stdbool.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

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
    // Disable interrupts globally
    unsigned state = irq_disable();

    // Register the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler);

    // Enable the specific interrupt
    irq_enable(IRQ_NUM);

    // Restore the previous interrupt state
    irq_restore(state);

    // Simulate some work
    printf("Starting work...\n");
    xtimer_sleep(2);

    // Check if the interrupt was handled
    if (interrupt_handled) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
