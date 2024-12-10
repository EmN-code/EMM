
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

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
    irq_set_handler(IRQ_NUM, irq_handler, NULL);
    irq_enable(IRQ_NUM);

    // Simulate an interrupt by setting the atomic variable
    atomic_var = 0;
    printf("Atomic variable set to 0\n");

    // Disable interrupts
    unsigned state = irq_disable();
    printf("Interrupts disabled\n");

    // Simulate some work
    xtimer_sleep(1);

    // Restore interrupts
    irq_restore(state);
    printf("Interrupts restored\n");

    // Check if the interrupt handler was called
    if (atomic_var == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    return 0;
}
