
#include <stdio.h>
#include <irq.h>
#include <xtimer.h>

volatile unsigned int atomic_var = 0;

void irq_handler(void)
{
    atomic_var++;
    printf("Interrupt handled, atomic_var incremented to: %u\n", atomic_var);
}

int main(void)
{
    unsigned state;

    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    state = irq_disable();
    printf("Interrupts disabled, state: %u\n", state);

    // Simulate some work with interrupts disabled
    xtimer_sleep(2);

    // Restore interrupts to their previous state
    irq_restore(state);
    printf("Interrupts restored, state: %u\n", state);

    // Enable the interrupt
    irq_set(0, irq_handler);
    printf("Interrupt set\n");

    // Simulate some work with interrupts enabled
    xtimer_sleep(5);

    printf("Test completed, final value of atomic_var: %u\n", atomic_var);

    return 0;
}
