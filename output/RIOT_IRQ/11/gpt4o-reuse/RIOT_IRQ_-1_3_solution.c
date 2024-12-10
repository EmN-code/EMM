
#include <stdio.h>
#include <stdint.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define IRQ_LINE (11)  // Example IRQ line

static volatile uint32_t interrupt_count = 0;

static void irq_handler(void *arg)
{
    (void)arg;
    interrupt_count++;
    printf("Interrupt handled, count: %lu\n", (unsigned long)interrupt_count);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the IRQ handler
    irq_disable(IRQ_LINE);
    irq_set_handler(IRQ_LINE, irq_handler, NULL);
    irq_enable(IRQ_LINE);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Main loop iteration: %d\n", i);
    }

    // Disable the IRQ
    irq_disable(IRQ_LINE);
    printf("IRQ disabled, final count: %lu\n", (unsigned long)interrupt_count);

    return 0;
}
