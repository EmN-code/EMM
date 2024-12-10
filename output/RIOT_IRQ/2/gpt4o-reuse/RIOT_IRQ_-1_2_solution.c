
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE 11

static volatile int atomic_var = 0;

static void irq_handler(void *arg)
{
    (void)arg;
    atomic_var = 1; // Set atomic variable to indicate interrupt occurred
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_disable(IRQ_LINE);
    irq_set(IRQ_LINE, irq_handler, NULL);
    irq_enable(IRQ_LINE);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Working... %d\n", i);
    }

    // Disable interrupts and check atomic variable
    irq_disable(IRQ_LINE);
    if (atomic_var == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    // Clean up
    irq_disable(IRQ_LINE);
    irq_clear(IRQ_LINE);

    printf("End of interrupt handling test\n");
    return 0;
}
