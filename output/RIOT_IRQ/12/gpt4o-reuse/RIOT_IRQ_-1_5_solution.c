
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

static volatile int atomic_var = 0;

void irq_handler(void)
{
    atomic_var = 1;
    printf("Interrupt handled, atomic_var set to 1\n");
}

int main(void)
{
    unsigned state;

    printf("Starting interrupt handling test\n");

    // Disable interrupts
    state = irq_disable();

    // Simulate some critical section code
    atomic_var = 0;
    printf("Atomic variable set to 0 in critical section\n");

    // Enable interrupts
    irq_restore(state);

    // Simulate an interrupt occurring
    irq_handler();

    // Verify the atomic variable was set by the interrupt handler
    if (atomic_var == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt handling failed\n");
    }

    return 0;
}
