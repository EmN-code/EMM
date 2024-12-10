
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <unistd.h>
#include <stddef.h>
#include <kernel_defines.h>

// Define a custom interrupt handler function
void custom_interrupt_handler(void)
{
    printf("Interrupt handled!\n");
}

int main(void)
{
    // Initialize an atomic variable to track interrupt state
    atomic_int interrupt_state = ATOMIC_VAR_INIT(0);

    // Disable interrupts and store the previous state
    unsigned old_state = irq_disable();

    // Set the custom interrupt handler for a specific IRQ number
    irq_set_exclusive_handler(UART_IRQ_NUM, custom_interrupt_handler);

    // Enable interrupts
    irq_enable();

    // Restore the previous interrupt state
    irq_restore(old_state);

    // Verify if disabling and restoring interrupts works correctly
    if (atomic_load(&interrupt_state) == 0) {
        printf("Interrupts were successfully disabled and restored.\n");
    } else {
        printf("Interrupts were not properly managed.\n");
    }

    return 0;
}
