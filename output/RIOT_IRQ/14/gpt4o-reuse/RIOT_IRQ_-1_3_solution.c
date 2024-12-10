
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE 11

volatile int atomic_var = 0;

void irq_handler(void *arg)
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
    irq_set_handler(IRQ_LINE, irq_handler, NULL);
    irq_enable(IRQ_LINE);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Working... atomic_var is %d\n", atomic_var);
        xtimer_sleep(1);
    }

    // Disable the interrupt
    irq_disable(IRQ_LINE);
    printf("Interrupt disabled\n");

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Working... atomic_var is %d\n", atomic_var);
        xtimer_sleep(1);
    }

    // Re-enable the interrupt
    irq_enable(IRQ_LINE);
    printf("Interrupt re-enabled\n");

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Working... atomic_var is %d\n", atomic_var);
        xtimer_sleep(1);
    }

    printf("End of interrupt handling test\n");
    return 0;
}
