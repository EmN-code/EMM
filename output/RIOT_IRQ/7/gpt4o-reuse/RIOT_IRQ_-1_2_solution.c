
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "irq.h"
#include "thread.h"
#include "xtimer.h"

#define IRQ_NUM 11

static volatile bool interrupt_occurred = false;

static void irq_handler(void *arg)
{
    (void)arg;
    interrupt_occurred = true;
    printf("Interrupt occurred\n");
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
        xtimer_sleep(1);
        printf("Working... %d\n", i);
    }

    // Disable interrupts and check if the flag is set
    irq_disable(IRQ_NUM);
    if (interrupt_occurred) {
        printf("Interrupt occurred while interrupts were disabled\n");
    } else {
        printf("No interrupt occurred while interrupts were disabled\n");
    }

    // Re-enable interrupts
    irq_enable(IRQ_NUM);

    // Simulate more work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Working... %d\n", i);
    }

    // Final check
    if (interrupt_occurred) {
        printf("Interrupt occurred as expected\n");
    } else {
        printf("Interrupt did not occur\n");
    }

    return 0;
}
