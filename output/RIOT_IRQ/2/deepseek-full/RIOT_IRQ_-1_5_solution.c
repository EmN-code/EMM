
#include <stdbool.h>
#include <stdlib.h>
#include <kernel_defines.h>
#include <stddef.h>
#include <stdio.h>
#include <stdatomic.h>
#include <irq.h>

// Define a flag to indicate interrupt occurrence
atomic_bool interrupt_occurred = ATOMIC_VAR_INIT(false);

// Function to handle the interrupt
void my_interrupt_handler(void)
{
    // Set the interrupt flag
    atomic_store(&interrupt_occurred, true);
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
        if (atomic_load(&interrupt_occurred)) {
            printf("Interrupt occurred!\n");
            // Reset the interrupt flag
            atomic_store(&interrupt_occurred, false);
        }
    }

    return 0;
}
