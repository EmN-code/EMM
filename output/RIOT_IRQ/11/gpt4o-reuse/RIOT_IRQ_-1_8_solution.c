
#include <stdio.h>
#include <stdint.h>
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
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_var = 0;

    // Restore interrupts
    irq_restore(state);

    // Wait for the interrupt to be handled
    xtimer_sleep(1);

    // Check if the interrupt was handled
    if (atomic_var == 1) {
        printf("Interrupt handling test passed\n");
    } else {
        printf("Interrupt handling test failed\n");
    }

    return 0;
}
