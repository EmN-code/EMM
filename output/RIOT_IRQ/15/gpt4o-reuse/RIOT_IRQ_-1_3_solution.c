
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE 11

static volatile uint32_t atomic_var = 0;

static void irq_handler(void *arg)
{
    (void)arg;
    atomic_var++;
    printf("Interrupt handled, atomic_var incremented to: %lu\n", (unsigned long)atomic_var);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Initialize the interrupt line
    irq_disable(IRQ_LINE);
    irq_set_handler(IRQ_LINE, irq_handler, NULL);
    irq_enable(IRQ_LINE);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Main thread working, atomic_var: %lu\n", (unsigned long)atomic_var);
    }

    // Disable the interrupt line
    irq_disable(IRQ_LINE);
    printf("Interrupt line disabled, atomic_var: %lu\n", (unsigned long)atomic_var);

    // Re-enable the interrupt line
    irq_enable(IRQ_LINE);
    printf("Interrupt line re-enabled, atomic_var: %lu\n", (unsigned long)atomic_var);

    // Simulate more work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Main thread working, atomic_var: %lu\n", (unsigned long)atomic_var);
    }

    printf("Interrupt handling test completed.\n");
    return 0;
}
