
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate interrupt handling
void interrupt_handler(void)
{
    // Increment the atomic variable
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    // Enable interrupts
    unsigned state = irq_enable();

    // Simulate an interrupt by calling the handler directly
    interrupt_handler();

    // Restore the previous interrupt state
    irq_restore(state);

    // Print the value of the atomic variable to verify the interrupt handling
    printf("Atomic variable value: %d\n", atomic_load(&atomic_var));

    return 0;
}
