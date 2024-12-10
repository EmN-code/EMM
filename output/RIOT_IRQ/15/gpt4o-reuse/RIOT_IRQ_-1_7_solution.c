
#include <stdio.h>
#include <xtimer.h>
#include <irq.h>

volatile int atomic_var = 0;

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

    // Set the interrupt handler
    irq_set_handler(0, irq_handler);

    // Restore interrupts
    irq_restore(state);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Working... atomic_var: %d\n", atomic_var);
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    // Disable interrupts again to check if atomic_var remains unchanged
    state = irq_disable();
    if (atomic_var == 1) {
        printf("Interrupts were correctly disabled and restored\n");
    } else {
        printf("Interrupts were not correctly handled\n");
    }
    irq_restore(state);

    return 0;
}
