
#include <stdio.h>
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
    irq_set(IRQ_NUM, irq_handler, NULL);
    irq_enable(IRQ_NUM);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        printf("Working... atomic_var is %d\n", atomic_var);
        xtimer_sleep(1);
    }

    // Disable the interrupt
    irq_disable(IRQ_NUM);
    printf("Interrupt disabled\n");

    // Simulate some more work
    for (int i = 0; i < 10; i++) {
        printf("Working... atomic_var is %d\n", atomic_var);
        xtimer_sleep(1);
    }

    // Re-enable the interrupt
    irq_enable(IRQ_NUM);
    printf("Interrupt re-enabled\n");

    // Simulate some more work
    for (int i = 0; i < 10; i++) {
        printf("Working... atomic_var is %d\n", atomic_var);
        xtimer_sleep(1);
    }

    printf("Interrupt handling test complete\n");
    return 0;
}
