
#include <stdio.h>
#include <irq.h>
#include <xtimer.h>
#include <thread.h>

#define IRQ_LINE 11

volatile int interrupt_occurred = 0;

void irq_handler(void *arg)
{
    (void)arg;
    interrupt_occurred = 1;
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_set_handler(IRQ_LINE, irq_handler, NULL);

    // Disable the interrupt
    irq_disable(IRQ_LINE);

    // Simulate some work
    xtimer_sleep(2);

    // Enable the interrupt
    irq_enable(IRQ_LINE);

    // Simulate some work
    xtimer_sleep(2);

    // Check if the interrupt occurred
    if (interrupt_occurred) {
        printf("Interrupt was successfully handled\n");
    } else {
        printf("Interrupt did not occur\n");
    }

    return 0;
}
