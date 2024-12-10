
#include <stdio.h>
#include <xtimer.h>
#include <irq.h>

volatile unsigned int atomic_var = 0;

void irq_handler(void)
{
    atomic_var = 1;
    printf("Interrupt handled, atomic_var set to 1\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set up the interrupt handler
    irq_set(0, irq_handler);

    // Restore interrupts
    irq_restore(state);

    // Simulate some work
    xtimer_sleep(2);

    // Check the atomic variable
    if (atomic_var == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    return 0;
}
