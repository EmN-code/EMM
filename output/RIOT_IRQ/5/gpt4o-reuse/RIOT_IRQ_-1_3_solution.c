
#include <stdio.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define IRQ_NUM 11

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
    irq_set(IRQ_NUM, irq_handler, NULL);

    // Disable the interrupt
    irq_disable(IRQ_NUM);

    // Enable the interrupt
    irq_enable(IRQ_NUM);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Working... %d\n", i);
        xtimer_sleep(1);
    }

    // Check if the interrupt was handled
    if (atomic_var == 1) {
        printf("Interrupt was successfully handled\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    // Clear the interrupt
    irq_clear(IRQ_NUM);

    printf("End of interrupt handling test\n");
    return 0;
}
