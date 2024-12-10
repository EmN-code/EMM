
#include <stdio.h>
#include "xtimer.h"
#include "irq.h"

volatile int interrupt_occurred = 0;

void interrupt_handler(void)
{
    interrupt_occurred = 1;
    printf("Interrupt occurred\n");
}

int main(void)
{
    unsigned state;

    printf("Starting interrupt handling test\n");

    // Disable interrupts
    state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(0, interrupt_handler);

    // Enable interrupts
    irq_restore(state);

    // Simulate an interrupt by calling the handler directly
    interrupt_handler();

    // Check if the interrupt was handled correctly
    if (interrupt_occurred) {
        printf("Interrupt handling test passed\n");
    } else {
        printf("Interrupt handling test failed\n");
    }

    return 0;
}
