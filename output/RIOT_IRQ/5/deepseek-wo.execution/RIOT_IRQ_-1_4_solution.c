
#include <stdlib.h>
#include <kernel_defines.h>
#include <irq.h>
#include <stddef.h>
#include <stdio.h>
#include <mma8x5x.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <mma8x5x_regs.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate an interrupt handler
void interrupt_handler(void)
{
    // Increment the atomic variable
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    // Initial state of interrupts
    unsigned int state = irq_disable();

    // Simulate some work
    printf("Disabling interrupts...\n");

    // Enable interrupts
    irq_restore(state);

    // Simulate an interrupt
    interrupt_handler();

    // Check the atomic variable
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupts were restored correctly.\n");
    } else {
        printf("Interrupts were not restored correctly.\n");
    }

    return 0;
}
