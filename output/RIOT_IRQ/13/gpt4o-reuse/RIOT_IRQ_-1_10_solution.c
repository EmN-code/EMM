
#include <stdio.h>
#include <stdint.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

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

    // Register the interrupt handler
    irq_disable();
    irq_set_handler(IRQ_LINE, irq_handler, NULL);
    irq_enable();

    // Test disabling and enabling interrupts
    printf("Disabling interrupts...\n");
    irq_disable();
    xtimer_sleep(2);
    printf("Atomic variable value after disabling interrupts: %lu\n", (unsigned long)atomic_var);

    printf("Enabling interrupts...\n");
    irq_enable();
    xtimer_sleep(2);
    printf("Atomic variable value after enabling interrupts: %lu\n", (unsigned long)atomic_var);

    printf("Interrupt handling test completed.\n");
    return 0;
}
