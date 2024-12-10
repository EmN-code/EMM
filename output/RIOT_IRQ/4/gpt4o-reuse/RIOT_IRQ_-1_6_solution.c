
#include <stdio.h>
#include "irq.h"
#include "xtimer.h"

MODULE_LICENSE("GPL");

static volatile int atomic_var = 0;

static void irq_handler(void)
{
    atomic_var = 1; // Set atomic variable to indicate interrupt occurred
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate some critical section code
    for (int i = 0; i < 1000000; i++) {
        // Dummy loop
    }

    // Restore interrupts to the previous state
    irq_restore(state);

    // Enable interrupts
    irq_enable();

    // Simulate an interrupt occurring
    irq_handler();

    // Check if the atomic variable was set
    if (atomic_var == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    return 0;
}
