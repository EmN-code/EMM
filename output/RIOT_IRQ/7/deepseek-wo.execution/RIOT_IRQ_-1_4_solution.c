
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Define a simple interrupt handler
void my_interrupt_handler(void)
{
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_exclusive_handler(UART_IRQ_NUM, my_interrupt_handler);

    // Enable interrupts
    irq_enable(UART_IRQ_NUM);

    // Restore the previous interrupt state
    irq_restore(state);

    // Main loop to check the atomic variable
    while (1) {
        if (atomic_load(&atomic_var) > 0) {
            printf("Interrupt handled! Atomic variable: %d\n", atomic_load(&atomic_var));
            atomic_store(&atomic_var, 0);
        }
    }

    return 0;
}
