
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <xtensa/hal.h>
#include <xtensa/core-macros.h>

volatile bool interrupt_occurred = false;
volatile bool interrupt_handled = false;

void interrupt_handler(void *arg)
{
    (void)arg;
    interrupt_occurred = true;
    interrupt_handled = true;
}

int main(void)
{
    // Initialize the interrupt handler
    xt_set_interrupt_handler(1, interrupt_handler, NULL);

    // Enable the interrupt
    xt_ints_on(1 << 1);

    // Simulate an interrupt by triggering it
    XT_SET_INTSET(1 << 1);

    // Wait for the interrupt to be handled
    while (!interrupt_handled) {
        // Busy wait
    }

    // Disable interrupts
    xt_ints_off(1 << 1);

    // Verify if the interrupt was handled correctly
    if (interrupt_occurred) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
