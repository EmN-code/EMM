
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <event/thread.h>
#include <thread.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to handle the interrupt
void irq_handler(void)
{
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    // Enable interrupts
    unsigned state = irq_enable();

    // Simulate an interrupt by calling the handler directly
    irq_handler();

    // Restore the interrupt state
    irq_restore(state);

    // Check the atomic variable to verify the interrupt handling
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupt handling works correctly.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    return 0;
}
