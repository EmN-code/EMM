
#include <stdio.h>
#include <stdbool.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

// Atomic variable to verify interrupt handling
volatile bool interrupt_handled = false;

// Function to handle the interrupt
void my_interrupt_handler(void)
{
    // Set the atomic variable to true when interrupt is handled
    interrupt_handled = true;
}

int main(void)
{
    // Disable interrupts initially
    unsigned state = irq_disable();

    // Register the interrupt handler
    irq_set_handler(MY_IRQ_LINE, my_interrupt_handler);

    // Enable the interrupt
    irq_enable(MY_IRQ_LINE);

    // Restore the interrupt state
    irq_restore(state);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Working... %d\n", i);
        xtimer_sleep(1);
    }

    // Check if the interrupt was handled
    if (interrupt_handled) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
