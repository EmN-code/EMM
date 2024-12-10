
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdatomic.h>
#include <irq.h>

// Define a global atomic variable to track interrupt state
static atomic_bool interrupt_flag = ATOMIC_VAR_INIT(false);

// Interrupt handler function
void my_interrupt_handler(void)
{
    // Set the interrupt flag to true
    atomic_store(&interrupt_flag, true);
}

int main(void)
{
    // Disable interrupts initially
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(MY_IRQ_LINE, my_interrupt_handler);

    // Enable interrupts
    irq_restore(state);

    // Main loop to check the interrupt flag
    while (1) {
        if (atomic_load(&interrupt_flag)) {
            printf("Interrupt handled!\n");
            // Reset the interrupt flag
            atomic_store(&interrupt_flag, false);
        }
    }

    return 0;
}
