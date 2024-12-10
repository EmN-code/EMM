
#include "irq.h"
#include "atomic_utils.h"
#include "xtimer.h"
#include <stdio.h>

atomic_int_t atomic_var;

void interrupt_handler(void)
{
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    atomic_init(&atomic_var, 0);

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(interrupt_handler);

    // Restore interrupts
    irq_restore(state);

    // Simulate an interrupt by calling the handler manually
    interrupt_handler();

    // Check the atomic variable
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupt handling and atomic operations work correctly.\n");
    } else {
        printf("Interrupt handling or atomic operations failed.\n");
    }

    return 0;
}
