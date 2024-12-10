
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

    // Disable interrupts to set up the interrupt handler
    state = irq_disable();

    // Set the custom interrupt handler
    irq_set_handler(IRQ_NUM, custom_interrupt_handler);

    // Restore the interrupt state
    irq_restore(state);

    // Enable interrupts
    irq_enable();

    // Main loop to check the interrupt flag
    while (1) {
        if (interrupt_flag) {
            printf("Interrupt handled successfully!\n");
            interrupt_flag = 0; // Reset the flag
        }
    }

    return 0;
}
