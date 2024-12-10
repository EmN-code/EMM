
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include "irq.h"
#include "thread.h"

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to handle the interrupt
void irq_handler(void)
{
    // Increment the atomic variable
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler);

    // Enable the interrupt
    irq_enable(IRQ_NUM);

    // Main loop
    while (1) {
        // Disable interrupts
        irq_disable();

        // Check the atomic variable
        if (atomic_load(&atomic_var) > 0) {
            printf("Interrupts were handled while disabled!\n");
        }

        // Restore interrupts
        irq_restore(irq_enable());

        // Sleep for a while
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
