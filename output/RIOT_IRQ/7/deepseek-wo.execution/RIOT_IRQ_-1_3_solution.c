
#include <stdio.h>
#include <stdbool.h>
#include "irq.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "thread.h"

volatile bool interrupt_occurred = false;

void my_interrupt_handler(void) {
    interrupt_occurred = true;
}

int main(void) {
    unsigned state;

    // Disable interrupts and save the state
    state = irq_disable();

    // Set the interrupt handler
    irq_set_exclusive_handler(UART_IRQ_NUM, my_interrupt_handler);

    // Enable the interrupt
    irq_enable(UART_IRQ_NUM);

    // Restore the interrupt state
    irq_restore(state);

    // Main loop to check if the interrupt occurred
    while (1) {
        if (interrupt_occurred) {
            printf("Interrupt occurred!\n");
            interrupt_occurred = false;
        }
        // Add a small delay to avoid busy-waiting
        thread_sleep();
    }

    return 0;
}
