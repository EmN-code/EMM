
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <event/thread.h>

volatile uint32_t atomic_var = 0;

void irq_handler(void)
{
    atomic_var = 1;
}

int main(void)
{
    unsigned state = irq_disable();

    // Set the exclusive IRQ handler
    irq_set_exclusive_handler(0, irq_handler);

    // Enable the IRQ
    irq_enable(0);

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Busy loop
    }

    // Restore the IRQ state
    irq_restore(state);

    // Check the atomic variable
    if (atomic_var == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
