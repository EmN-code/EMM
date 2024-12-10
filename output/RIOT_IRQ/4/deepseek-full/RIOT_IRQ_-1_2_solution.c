
#include "mma8x5x_regs.h"
#include "irq.h"

volatile int interrupt_flag = 0;

void interrupt_handler(void) {
    interrupt_flag = 1;
}

int main(void) {
    unsigned state;

    // Register the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler);

    // Disable interrupts and save the state
    state = irq_disable();

    // Simulate some critical section code
    for (int i = 0; i < 1000000; i++) {
        // Some critical code here
    }

    // Restore the interrupt state
    irq_restore(state);

    // Enable the interrupt
    irq_enable();

    // Simulate an interrupt trigger
    // This part would typically be replaced by actual hardware triggering
    // For simulation purposes, we manually set the interrupt flag
    interrupt_flag = 1;

    // Wait for the interrupt to be handled
    while (interrupt_flag == 1) {
        // Busy wait
    }

    // Check if the interrupt was handled correctly
    if (interrupt_flag == 0) {
        printf("Interrupt handled successfully!\n");
    } else {
        printf("Interrupt handling failed!\n");
    }

    return 0;
}
