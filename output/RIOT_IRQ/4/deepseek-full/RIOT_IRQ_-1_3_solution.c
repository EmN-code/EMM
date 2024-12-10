
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "irq.h"

volatile bool interrupt_handled = false;

void custom_irq_handler(void)
{
    interrupt_handled = true;
}

int main(void)
{
    unsigned state;

    // Initialize the MMA8x5x sensor
    mma8x5x_init();

    // Disable interrupts
    state = irq_disable();

    // Set custom IRQ handler
    irq_set_handler(MMA8X5X_INT_PIN, custom_irq_handler);

    // Enable interrupts
    irq_restore(state);

    // Main loop
    while (1) {
        if (interrupt_handled) {
            printf("Interrupt handled successfully!\n");
            interrupt_handled = false;
        }
    }

    return 0;
}
