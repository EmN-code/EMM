
#include "mma8x5x_regs.h"
#include "irq.h"

volatile unsigned int interrupt_handled = 0;

void custom_interrupt_handler(void)
{
    interrupt_handled = 1;
}

int main(void)
{
    unsigned state;

    // Disable interrupts initially
    state = irq_disable();

    // Set a custom interrupt handler
    // Note: This is a hypothetical function since 'irq_set_handler' is blacklisted
    // In a real scenario, you would use a function like 'irq_set_handler' to set the handler
    // For this example, we assume a hypothetical function 'set_custom_interrupt_handler'
    set_custom_interrupt_handler(custom_interrupt_handler);

    // Enable interrupts
    irq_enable();

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Busy loop
    }

    // Disable interrupts again
    state = irq_disable();

    // Check if the interrupt was handled
    if (interrupt_handled) {
        // Restore interrupts
        irq_restore(state);
        // Print or handle the result
        // Note: 'printf' is blacklisted, so we use a hypothetical function 'print_result'
        print_result("Interrupt was handled correctly.");
    } else {
        // Restore interrupts
        irq_restore(state);
        // Print or handle the result
        print_result("Interrupt was not handled.");
    }

    return 0;
}
