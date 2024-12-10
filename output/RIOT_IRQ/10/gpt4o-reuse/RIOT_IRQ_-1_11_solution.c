
#include <stdio.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define IRQ_LINE (11)

static volatile int atomic_var = 0;

static void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1;
    printf("Interrupt handled, atomic_var set to 1\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_set_handler(IRQ_LINE, irq_handler, NULL);

    // Disable the interrupt
    irq_disable(IRQ_LINE);
    printf("Interrupt disabled\n");

    // Enable the interrupt
    irq_enable(IRQ_LINE);
    printf("Interrupt enabled\n");

    // Simulate some work
    xtimer_sleep(2);

    // Check the atomic variable
    if (atomic_var == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    // Disable the interrupt again
    irq_disable(IRQ_LINE);
    printf("Interrupt disabled again\n");

    return 0;
}
