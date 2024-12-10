
#include "mma8x5x_regs.h"
#include "irq.h"

volatile int interrupt_flag = 0;

void custom_interrupt_handler(void)
{
    interrupt_flag = 1;
}

int main(void)
{
    unsigned state;

    // Initialize the interrupt flag
    interrupt_flag = 0;

    // Disable interrupts to set up the handler
    state = irq_disable();

    // Set the custom interrupt handler
    irq_set_handler(IRQ_NUM, custom_interrupt_handler);

    // Restore the interrupt state
    irq_restore(state);

    // Enable the interrupt
    irq_enable();

    // Main loop to check the interrupt flag
    while (1) {
        if (interrupt_flag) {
            // Handle the interrupt
            // For demonstration, we just print a message
            // Note: printf is not allowed, so we use a placeholder
            // printf("Interrupt handled!\n");

            // Clear the interrupt flag
            interrupt_flag = 0;
        }
    }

    return 0;
}
