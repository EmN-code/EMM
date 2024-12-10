
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

    // Disable interrupts to set up the interrupt handler
    state = irq_disable();

    // Set the custom interrupt handler
    irq_set_handler(IRQ_NUM, custom_interrupt_handler);

    // Enable the interrupt
    irq_enable(IRQ_NUM);

    // Restore the interrupt state
    irq_restore(state);

    // Wait for the interrupt to be triggered
    while (interrupt_flag == 0) {
        // Busy wait
    }

    // Check if the interrupt was handled correctly
    if (interrupt_flag == 1) {
        printf("Interrupt was handled successfully!\n");
    } else {
        printf("Interrupt handling failed!\n");
    }

    return 0;
}
