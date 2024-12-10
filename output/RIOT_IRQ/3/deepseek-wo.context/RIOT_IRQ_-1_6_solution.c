
#include <stdio.h>
#include <stdint.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

// Define an atomic variable type
typedef volatile int atomic_int;

// Function prototypes
void interrupt_handler(void *arg);

// Global atomic variable
atomic_int atomic_var = 0;

int main(void)
{
    // Initialize the atomic variable
    atomic_var = 0;

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_var = 1;

    // Restore interrupts
    irq_restore(state);

    // Set an interrupt handler
    irq_set_handler(0, interrupt_handler, NULL);

    // Simulate some work
    xtimer_sleep(1);

    // Check the atomic variable
    if (atomic_var == 2) {
        printf("Interrupt handling and atomic operations worked correctly.\n");
    } else {
        printf("Interrupt handling or atomic operations failed.\n");
    }

    return 0;
}

// Interrupt handler function
void interrupt_handler(void *arg)
{
    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_var = 2;

    // Restore interrupts
    irq_restore(state);
}
