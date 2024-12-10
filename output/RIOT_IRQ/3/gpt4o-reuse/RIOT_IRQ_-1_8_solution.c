
#include <stdio.h>
#include <stdint.h>
#include "xtimer.h"
#include "thread.h"
#include "irq.h"

#define IRQ_NUM 11

static volatile int atomic_var = 0;

static void irq_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
    atomic_var = 1;
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Register the interrupt handler
    irq_disable(IRQ_NUM);
    irq_set(IRQ_NUM, irq_handler, NULL);
    irq_enable(IRQ_NUM);

    // Test disabling and restoring interrupts
    printf("Disabling interrupts...\n");
    irq_disable(IRQ_NUM);
    xtimer_sleep(2);
    if (atomic_var == 0) {
        printf("Interrupts disabled correctly.\n");
    } else {
        printf("Interrupts were not disabled correctly.\n");
    }

    printf("Restoring interrupts...\n");
    irq_enable(IRQ_NUM);
    xtimer_sleep(2);
    if (atomic_var == 1) {
        printf("Interrupts restored correctly.\n");
    } else {
        printf("Interrupts were not restored correctly.\n");
    }

    // Clean up
    irq_disable(IRQ_NUM);
    irq_clear(IRQ_NUM);

    printf("Interrupt handling test completed.\n");
    return 0;
}
