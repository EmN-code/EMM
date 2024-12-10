
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <kernel_defines.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate interrupt handling
void interrupt_handler(void)
{
    atomic_fetch_add(&atomic_var, 1);
    printf("Interrupt handled, atomic_var incremented to: %d\n", atomic_var);
}

// Main function
int main(void)
{
    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate some work
    printf("Disabling interrupts...\n");
    xtimer_sleep(1);

    // Enable interrupts
    irq_restore(state);

    // Simulate an interrupt
    interrupt_handler();

    // Verify the atomic variable
    if (atomic_var == 1) {
        printf("Interrupts were disabled and restored correctly.\n");
    } else {
        printf("Interrupts were not handled correctly.\n");
    }

    return 0;
}
