
#include <stdio.h>
#include <stdbool.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

// Atomic variable to verify interrupt handling
volatile bool interrupt_handled = false;

// Function to be called when an interrupt occurs
void interrupt_handler(void)
{
    interrupt_handled = true;
    printf("Interrupt handled!\n");
}

int main(void)
{
    // Disable interrupts initially
    unsigned state = irq_disable();

    // Set up the interrupt handler
    irq_set_handler(interrupt_handler);

    // Enable interrupts
    irq_restore(state);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Working... %d\n", i);
        xtimer_sleep(1);
    }

    // Check if the interrupt was handled
    if (interrupt_handled) {
        printf("Interrupt was successfully handled.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
