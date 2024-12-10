
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
    irq_disable(IRQ_NUM);
    irq_set(IRQ_NUM, irq_handler, NULL);
    irq_enable(IRQ_NUM);

    // Simulate an interrupt by setting the atomic variable
    atomic_var = 0;
    printf("Atomic variable set to 0\n");

    // Disable interrupts and check the atomic variable
    unsigned state = irq_disable();
    if (atomic_var == 0) {
        printf("Interrupts disabled, atomic_var is 0\n");
    } else {
        printf("Interrupts disabled, atomic_var is 1 (unexpected)\n");
    }

    // Restore interrupts and check the atomic variable
    irq_restore(state);
    if (atomic_var == 0) {
        printf("Interrupts restored, atomic_var is 0 (unexpected)\n");
    } else {
        printf("Interrupts restored, atomic_var is 1\n");
    }

    // Wait for a while to allow the interrupt to be handled
    xtimer_sleep(1);

    // Final check of the atomic variable
    if (atomic_var == 1) {
        printf("Interrupt handling verified, atomic_var is 1\n");
    } else {
        printf("Interrupt handling failed, atomic_var is 0 (unexpected)\n");
    }

    return 0;
}
