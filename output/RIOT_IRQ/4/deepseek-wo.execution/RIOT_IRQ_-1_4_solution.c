
#include "kernel_defines.h"
#include "irq.h"
#include "stddef.h"
#include "stdio.h"
#include "mma8x5x.h"
#include "stdbool.h"
#include "mma8x5x_regs.h"
#include "stdlib.h"
#include "stdatomic.h"

// Define an atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to handle the interrupt
void my_interrupt_handler(void)
{
    // Increment the atomic variable
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    // Disable interrupts initially
    unsigned state = irq_disable();

    // Set up the interrupt handler
    // Note: This is a placeholder for actual interrupt setup code
    // which would depend on the specific hardware and driver used.
    // For example, if using UART, you would typically set up the UART
    // interrupt handler here.

    // Enable interrupts
    irq_restore(state);

    // Main loop to check the atomic variable
    while (1) {
        // Simulate some work
        printf("Atomic variable value: %d\n", atomic_load(&atomic_var));

        // Wait for a while
        for (volatile int i = 0; i < 1000000; i++) {}
    }

    return 0;
}
