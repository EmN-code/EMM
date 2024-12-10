
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

#define IRQ_LINE 11

static volatile int atomic_var = 0;

static void irq_handler(void *arg)
{
    (void)arg;
    atomic_var++;
    printf("Interrupt handled, atomic_var incremented to: %d\n", atomic_var);
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
        printf("Main loop iteration: %d\n", i);
    }

    // Disable the interrupt
    irq_disable(IRQ_LINE);
    printf("Interrupt disabled, atomic_var: %d\n", atomic_var);

    // Simulate some more work
    for (int i = 0; i < 5; i++) {
        xtimer_sleep(1);
        printf("Main loop iteration after disabling interrupt: %d\n", i);
    }

    // Re-enable the interrupt
    irq_enable(IRQ_LINE);
    printf("Interrupt re-enabled\n");

    // Simulate some more work
    for (int i = 0; i < 5; i++) {
        xtimer_sleep(1);
        printf("Main loop iteration after re-enabling interrupt: %d\n", i);
    }

    printf("Interrupt handling test completed.\n");
    return 0;
}
