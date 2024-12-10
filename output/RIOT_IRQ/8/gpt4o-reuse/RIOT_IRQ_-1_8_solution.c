
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE 11

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
    irq_disable(IRQ_LINE);
    irq_set(IRQ_LINE, irq_handler, NULL);
    irq_enable(IRQ_LINE);

    // Simulate an interrupt by setting the atomic variable
    atomic_var = 0;
    printf("Atomic variable set to 0\n");

    // Wait for the interrupt to be handled
    xtimer_sleep(1);

    // Check if the interrupt was handled correctly
    if (atomic_var == 1) {
        printf("Interrupt handling successful\n");
    } else {
        printf("Interrupt handling failed\n");
    }

    // Clean up
    irq_disable(IRQ_LINE);
    irq_clear(IRQ_LINE);

    return 0;
}
