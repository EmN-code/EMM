
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

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(IRQ_LINE, irq_handler, NULL);

    // Enable interrupts
    irq_restore(state);

    // Trigger the interrupt
    irq_trigger(IRQ_LINE);

    // Wait for the interrupt to be handled
    xtimer_sleep(1);

    // Check if the interrupt was handled correctly
    if (atomic_var == 1) {
        printf("Interrupt handling test passed\n");
    } else {
        printf("Interrupt handling test failed\n");
    }

    return 0;
}
