
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

    // Disable interrupts and check atomic variable
    irq_disable(IRQ_NUM);
    atomic_var = 0;
    xtimer_sleep(1); // Simulate some work
    if (atomic_var == 0) {
        printf("Interrupts disabled correctly\n");
    } else {
        printf("Interrupts were not disabled correctly\n");
    }
    irq_enable(IRQ_NUM);

    // Wait for interrupt to be handled
    xtimer_sleep(1);
    if (atomic_var == 1) {
        printf("Interrupts enabled correctly\n");
    } else {
        printf("Interrupts were not enabled correctly\n");
    }

    printf("Interrupt handling test complete\n");
    return 0;
}
